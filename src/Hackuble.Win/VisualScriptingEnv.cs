using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Hackuble.Win
{
    public partial class VisualScriptingEnv : Form
    {
        private Controls.OpenGLControl openGLControl1;
        public VisualScriptingEnv()
        {
            InitializeComponent();
        }

        private void VisualScriptingEnv_Load(object sender, EventArgs e)
        {
            LoadVSEControl();
        }

        private void LoadVSEControl()
        {
            this.openGLControl1 = new Win.Controls.OpenGLControl();
            this.openGLControl1.SuspendLayout();
            this.SuspendLayout();
            this.openGLControl1.BringToFront();
            this.openGLControl1.Size = new System.Drawing.Size((this.Size.Width), (this.Size.Height - (this.toolStrip1.Height + statusStrip1.Height)));
            this.openGLControl1.Location = new System.Drawing.Point(0, this.toolStrip1.Height);
            this.openGLControl1.Name = "openGLControl1";
            this.openGLControl1.TabIndex = 3;
            this.openGLControl1.ProgressBar = this.toolStripProgressBar1;
            this.openGLControl1.Paint += OpenGLControl1_Paint;
            this.Controls.Add(this.openGLControl1);
            this.openGLControl1.ResumeLayout(false);
            this.openGLControl1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout(this.openGLControl1, "Size");
        }

        private void OpenGLControl1_Paint(object sender, PaintEventArgs e)
        {
            toolStripStatusLabel1.Text = this.openGLControl1.FrameRate.ToString();
        }

        private void VisualScriptingEnv_Paint(object sender, PaintEventArgs e)
        {
            toolStripProgressBar1.Value = 50;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            this.openGLControl1.Refresh();
        }

        private void VisualScriptingEnv_Resize(object sender, EventArgs e)
        {
            this.openGLControl1.Size = new System.Drawing.Size((this.Size.Width), (this.Size.Height - (this.toolStrip1.Height + statusStrip1.Height)));
        }
    }
}
