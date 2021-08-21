﻿using System;
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
        public ToolStripProgressBar ProgressBar { get; set; }

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

        public List<VisualScripting.Component> sceneComponents;
        bool selected = false;
        VisualScripting.Component selectedComponent = null;
        Vector2 oldMouseWorld;

        public static bool InVisualStudio()
        {
            return StringComparer.OrdinalIgnoreCase.Equals(
              "devenv", Process.GetCurrentProcess().ProcessName);
            
        }
        public OpenGLControl(int width, int height)
        {
            InitializeComponent();

            sceneComponents = new List<VisualScripting.Component>();

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

                if(selected)
                {
                    selectedComponent.Translate(new CanvasPoint(mouseTranslate.X, mouseTranslate.Y));
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

                    foreach (VisualScripting.Component comp in sceneComponents)
                    {
                        if(comp.Bounds.Contains(new CanvasPoint(worldMouse.X, worldMouse.Y)))
                        {
                            selectedComponent = comp;
                            selected = true;
                        }
                    }
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
                    selected = false;
                    selectedComponent = null;
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

            if (!InVisualStudio())
            {
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
            }

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
}
