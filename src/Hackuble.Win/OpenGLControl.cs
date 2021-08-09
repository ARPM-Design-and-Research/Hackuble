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

namespace Hackuble.Win
{

    public partial class OpenGLControl : UserControl
    {
        //byte[] pixelData;
        ManagedContext context;

        int windowWidth;
        int windowHeight;
        bool initializedContext = false;
        //Bitmap drawingBitmap;

        public static bool InVisualStudio()
        {
            return StringComparer.OrdinalIgnoreCase.Equals(
              "devenv", Process.GetCurrentProcess().ProcessName);
            
        }
        public OpenGLControl()
        {
            InitializeComponent();

            if (!InVisualStudio())
            {
                windowWidth = this.Size.Width;
                windowHeight = this.Size.Height;

                //pixelData = new byte[4 * windowWidth * windowHeight];

                this.DoubleBuffered = true;
            }
        }

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);

            if (!InVisualStudio())
            {
                windowWidth = this.Size.Width;
                windowHeight = this.Size.Height;

                //pixelData = new byte[4 * windowWidth * windowHeight];

                if (!initializedContext)
                {
                    context = new ManagedContext();
                    context.createContext(windowWidth, windowHeight);
                    initializedContext = true;
                }
                else
                {
                    context.resize(this.Size.Width, this.Size.Height);
                }

                //drawingBitmap = new Bitmap(windowWidth, windowHeight, PixelFormat.Format32bppArgb);
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
                context.onMouseWheel(e.Delta);

            this.Invalidate();
        }

        protected override void OnPaint(PaintEventArgs e)
        {

            base.OnPaint(e);

            if (!InVisualStudio())
            {
                context.onPaint();


                var area = new Rectangle(new Point(0, 0), new Size(windowWidth, windowHeight));

                using (Bitmap b = new Bitmap(windowWidth, windowHeight))
                {
                    byte[] pixelData = new byte[4 * windowWidth * windowHeight];
                    context.getPixelData(ref pixelData);
                    b.SetResolution(windowWidth, windowHeight);
                    using (Graphics g = Graphics.FromImage(b))
                    {
                        g.Clear(Color.Red);
                        //draw each pixel
                        int position = 0;
                        for (int x = 0; x < b.Width; x++)
                        {
                            for (int y = 0; y < b.Height; y++)
                            {
                                Color newColor = Color.FromArgb(BitConverter.ToInt32(pixelData, (position * 4)));
                                b.SetPixel(x, y, newColor);
                                position++;
                            }
                        }
                    }
                    e.Graphics.DrawImage(b, area);
                }

                //context.getPixelData(ref pixelData);
                //UpdateBitmap(pixelData, windowWidth, windowHeight, drawingBitmap);
                //e.Graphics.DrawImage(drawingBitmap, area);
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
