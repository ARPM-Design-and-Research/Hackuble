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
        Bitmap drawingBitmap;

        public static bool InVisualStudio()
        {
            return StringComparer.OrdinalIgnoreCase.Equals(
              "devenv", Process.GetCurrentProcess().ProcessName);
            
        }
        public OpenGLControl(int width, int height)
        {
            InitializeComponent();

            this.Size = new System.Drawing.Size(width, height);

            if (!InVisualStudio())
            {
                windowWidth = this.Size.Width;
                windowHeight = this.Size.Height;

                pixelData = new byte[4 * windowWidth * windowHeight];

                context = new ManagedContext();
                context.createContext(windowWidth, windowHeight);
                context.awaitInitialized();

                drawingBitmap = new Bitmap(windowWidth, windowHeight, PixelFormat.Format32bppArgb);

                this.DoubleBuffered = true;
            }

            VisualScripting.Component comp = new VisualScripting.Component();
            comp.setColor(255, 0, 0);
            comp.setName("From C#");
            comp.addSlider("Slider 1", 0.5f, 0.0f, 1.0f);
        }

        public void resizeOpenGL(EventArgs e)
        {
            if (!InVisualStudio())
            {

                windowWidth = this.Size.Width;
                windowHeight = this.Size.Height;

                context.resize(windowWidth, windowHeight);
                context.awaitResize();

                pixelData = new byte[4 * windowWidth * windowHeight];

                drawingBitmap = new Bitmap(windowWidth, windowHeight, PixelFormat.Format32bppArgb);
            }
        }

        protected override void OnMouseHover(EventArgs e)
        {
            base.OnMouseHover(e);

            this.Invalidate();
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (!InVisualStudio())
                context.onMouseMove(e.X, e.Y, (int)e.Button);

            this.Invalidate();
        }

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            base.OnKeyPress(e);

        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseClick(e);

            if (!InVisualStudio())
                context.onMouseDown(e.X, e.Y, (int)e.Button);

            this.Invalidate();
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);

            if (!InVisualStudio())
                context.onMouseUp(e.X, e.Y, (int)e.Button);

            this.Invalidate();
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);

            if (!InVisualStudio())
                context.onMouseWheel(e.X, e.Y, e.Delta);

            this.Invalidate();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (!InVisualStudio())
            {
                //context.onPaint();

                var area = new Rectangle(new Point(0, 0), new Size(windowWidth, windowHeight));

                //if (this.ProgressBar != null)
                //{
                //    // set maximum
                //    this.ProgressBar.Maximum = windowWidth * windowHeight;
                //    this.ProgressBar.Value = 0;
                //}

                //using (Bitmap b = new Bitmap(windowWidth, windowHeight))
                //{
                //    byte[] pixelData = new byte[4 * windowWidth * windowHeight];
                //    context.getPixelData(ref pixelData);
                //    b.SetResolution(windowWidth, windowHeight);
                //    using (Graphics g = Graphics.FromImage(b))
                //    {
                //        g.Clear(Color.Red);
                //        //draw each pixel
                //        int position = 0;
                //        for (int y = 0; y < b.Height; y++)
                //        {
                //            for (int x = 0; x < b.Width; x++)
                //            {
                //                Color newColor = Color.FromArgb(BitConverter.ToInt32(pixelData, (position * 4)));
                //                b.SetPixel(x, y, newColor);
                //                position++;
                //                //if (this.ProgressBar != null)
                //                //{
                //                //    //set value
                //                //    this.ProgressBar.Value = position;
                //                //}
                //            }
                //        }
                //    }
                //    b.RotateFlip(RotateFlipType.RotateNoneFlipY);
                //    e.Graphics.DrawImage(b, area);
                //}

                context.getPixelData(ref pixelData);
                UpdateBitmap(pixelData, windowWidth, windowHeight, drawingBitmap);
                e.Graphics.DrawImage(drawingBitmap, area);
            }

            //calc fps
            _framesRendered++;
            if ((DateTime.Now - _lastTime).TotalSeconds >= 1)
            {
                // one second has elapsed 
                _fps = _framesRendered;
                _framesRendered = 0;
                _lastTime = DateTime.Now;
            }
        }


        private static void UpdateBitmap(byte[] arr, int width, int height, Bitmap bit)
        {
            //var output = new Bitmap(width, height, pixelFormat);
            var rect = new Rectangle(0, 0, width, height);
            var bmpData = bit.LockBits(rect, ImageLockMode.ReadWrite, bit.PixelFormat);

            // Row-by-row copy
            var arrRowLength = width * Image.GetPixelFormatSize(bit.PixelFormat) / 8;
            var ptr = bmpData.Scan0;
            for (var i = height - 1; i >= 0; i--)
            {
                Marshal.Copy(arr, i * arrRowLength, ptr, arrRowLength);
                ptr += bmpData.Stride;
            }
            //output.Save("test.png");
            bit.UnlockBits(bmpData);
        }


    }
}
