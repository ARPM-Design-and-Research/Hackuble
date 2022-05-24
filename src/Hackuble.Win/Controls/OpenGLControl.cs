using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Numerics;

using GUICLR;

using VisualScripting;

namespace Hackuble.Win.Controls
{

    public partial class OpenGLControl : UserControl
    {
        DateTime _lastTime;
        private int _fps;
        int _framesRendered;

        public int FrameRate
        {
            get
            {
                return _fps;
            }
        }

        byte[] pixelData;
        ManagedContext context;

        int windowWidth;
        int windowHeight;
        int pivotX;
        int pivotY;
        System.Drawing.Rectangle area;
        Bitmap drawingBitmap;

        //public List<VisualScripting.Component> ActiveComponents { get; private set; }
        public List<VisualScripting.Element> ActiveElements { get; private set; }

        public VisualScripting.Element ElementInFocus { get; private set; }
        //bool selected = false;
        //VisualScripting.Component selectedComponent = null;
        Vector2 oldMouseWorld;

        public event EventHandler<CanvasMouseDownEventArgs> ElementMouseDown;

        public void AddComponent(VisualScripting.Component component)
        {
            this.ActiveElements.Add(component);
            foreach (VisualScripting.Element e in component.Children)
            {
                if (e != null) this.ActiveElements.Add(e);
            }

        }

        public static bool InVisualStudio()
        {
            return StringComparer.OrdinalIgnoreCase.Equals(
              "devenv", Process.GetCurrentProcess().ProcessName);
            
        }
        public OpenGLControl(int width, int height)
        {
            InitializeComponent();

            ZOrderManager.Initialize();

            ElementMouseDown += OpenGLControl_ElementMouseDown;

            //ActiveComponents = new List<VisualScripting.Component>();
            ActiveElements = new List<VisualScripting.Element>();

            oldMouseWorld = new Vector2(0);

            this.ClientSize = new System.Drawing.Size(width, height);

            if (!InVisualStudio())
            {
                windowWidth = this.ClientSize.Width;
                windowHeight = this.ClientSize.Height;
                pivotX = this.ClientRectangle.X;
                pivotY = this.ClientRectangle.Y;

                pixelData = new byte[4 * windowWidth * windowHeight];

                context = new ManagedContext();
                context.createContext(windowWidth, windowHeight);
                context.awaitInitialized();

                drawingBitmap = new Bitmap(windowWidth, windowHeight, PixelFormat.Format32bppArgb);

                this.DoubleBuffered = true;
            }

            context.displayTextBoundingBox(false);
        }

        private void OpenGLControl_ElementMouseDown(object sender, CanvasMouseDownEventArgs e)
        {
            if (e.Elements.Count > 0)
            {
                ElementInFocus = e.Elements[0];

                foreach (VisualScripting.Element possibleHit in e.Elements)
                {
                    if (possibleHit.ZOrder > ElementInFocus.ZOrder)
                        ElementInFocus = possibleHit;
                }

                //ElementInFocus has been defined.

                ZOrderManager.FocusOnElement(ElementInFocus);

                float topZOrder = 0.0f;

                foreach (VisualScripting.Element topElement in ActiveElements)
                {
                    if (topElement.ZOrder > topZOrder)
                        topZOrder = topElement.ZOrder;
                }

                for (int i = 0; i < ActiveElements.Count; i++)
                {
                    VisualScripting.Element otherElem = ActiveElements[i];

                    if (ElementInFocus != otherElem && otherElem.ZOrder > ElementInFocus.ZOrder)
                    {
                        otherElem.ZOrder -= ElementInFocus.ZDepth;
                    }
                }

                ElementInFocus.ZOrder = topZOrder;
            }
        }

        public void closeOpenGL()
        {
            context.closeContext();
        }

        public void resizeOpenGL(EventArgs e)
        {
            if (!InVisualStudio())
            {
                try
                {
                    if (pivotX != this.ClientRectangle.X || pivotY != this.ClientRectangle.Y)
                    {
                        pivotX = this.ClientRectangle.X;
                        pivotY = this.ClientRectangle.Y;
                    }
                    if (windowHeight != this.Size.Height || windowWidth != this.Size.Width)
                    {
                        windowWidth = this.ClientSize.Width;
                        windowHeight = this.ClientSize.Height;

                        context.resize(windowWidth, windowHeight);
                        context.awaitResize();

                        pixelData = new byte[4 * windowWidth * windowHeight];

                        drawingBitmap = new Bitmap(windowWidth, windowHeight, PixelFormat.Format32bppArgb);
                    }
                }
                catch (Exception ex)
                {
                    ex.ToString();
                    return;
                }
            }
        }

        protected override void OnMouseHover(EventArgs e)
        {
            base.OnMouseHover(e);

            if (!InVisualStudio())
                this.Refresh();
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (!InVisualStudio())
            {
                context.onMouseMove(e.X, e.Y, (int)e.Button);

                Vector2 worldMouse = context.screenToWorldSpace(new Vector2(e.X, e.Y));

                Vector2 mouseTranslate = worldMouse - oldMouseWorld;

                oldMouseWorld = worldMouse;

                if(e.Button == MouseButtons.Left && ElementInFocus != null)
                {
                    ElementInFocus.Translate(new CanvasPoint(mouseTranslate.X, mouseTranslate.Y));
                }

                this.Refresh();
            }
        }

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            base.OnKeyPress(e);

        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseClick(e);

            if (!InVisualStudio())
            {
                context.onMouseDown(e.X, e.Y, (int)e.Button);

                if(e.Button == MouseButtons.Left)
                {
                    Vector2 worldMouse = context.screenToWorldSpace(new Vector2(e.X, e.Y));

                    List<VisualScripting.Element> hitTestElements = new List<Element>();

                    foreach (VisualScripting.Element elem in ActiveElements)
                    {
                        if (elem.Bounds.Contains(new CanvasPoint(worldMouse.X, worldMouse.Y)))
                        {
                            hitTestElements.Add(elem);
                        }
                    }

                    ElementMouseDown.Invoke(this, new CanvasMouseDownEventArgs(e, worldMouse, hitTestElements));

                }
                else if (e.Button == MouseButtons.Right)
                {
                    this.Cursor = new Cursor(Properties.Resources.hand.Handle);
                }

                this.Refresh();
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);

            if (!InVisualStudio())
            { 
                context.onMouseUp(e.X, e.Y, (int)e.Button);

                if(e.Button == MouseButtons.Left)
                {
                    ElementInFocus = null;
                }
                else if (e.Button == MouseButtons.Right)
                {
                    this.Cursor = DefaultCursor;
                }

                this.Refresh();
            }
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);

            if (!InVisualStudio())
            {
                context.onMouseWheel(e.X, e.Y, e.Delta);

                this.Refresh();
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (InVisualStudio()) return;

            context.onPaint();

            //if (pivotX != this.ClientRectangle.X || pivotY != this.ClientRectangle.Y)
            //{
            //    pivotX = this.ClientRectangle.X;
            //    pivotY = this.ClientRectangle.Y;
            //}
            //if (windowHeight != this.ClientSize.Height || windowWidth != this.ClientSize.Width)
            //{
            //    windowWidth = this.ClientSize.Width;
            //    windowHeight = this.ClientSize.Height;
            //}
            area = new System.Drawing.Rectangle(new Point(pivotX, pivotY), new Size(windowWidth, windowHeight));

            context.getPixelData(ref pixelData);
            UpdateBitmap(pixelData, drawingBitmap);
            e.Graphics.DrawImage(drawingBitmap, area);


            //
            //      FPS CALCULATION
            //
            _framesRendered++;
            if ((DateTime.Now - _lastTime).TotalSeconds >= 1)
            {
                // one second has elapsed 
                _fps = _framesRendered;
                _framesRendered = 0;
                _lastTime = DateTime.Now;
            }
        }


        private static void UpdateBitmap(byte[] arr, Bitmap bit)
        {
            var rect = new System.Drawing.Rectangle(0, 0, bit.Width, bit.Height);
            var bmpData = bit.LockBits(rect, ImageLockMode.ReadWrite, bit.PixelFormat);

            // Row-by-row copy
            var arrRowLength = bit.Width * Image.GetPixelFormatSize(bit.PixelFormat) / 8;
            var ptr = bmpData.Scan0;
            for (var i = bit.Height - 1; i >= 0; i--)
            {
                Marshal.Copy(arr, i * arrRowLength, ptr, arrRowLength);
                ptr += bmpData.Stride;
            }
            bit.UnlockBits(bmpData);
        }


    }

    public class CanvasMouseDownEventArgs : MouseEventArgs
    {
        public List<VisualScripting.Element> Elements { get; set; }
        public Vector2 CanvasSpaceMousePosition { get; set; }

        public CanvasMouseDownEventArgs(MouseEventArgs e, Vector2 canvasSpaceMousePosition, List<VisualScripting.Element> elements) : base(e.Button, e.Clicks, e.X, e.Y, e.Delta)
        {
            if (elements != null && elements.Count > 0) this.Elements = elements;
            else this.Elements = new List<Element>();
            this.CanvasSpaceMousePosition = canvasSpaceMousePosition;
        }
    }
}
