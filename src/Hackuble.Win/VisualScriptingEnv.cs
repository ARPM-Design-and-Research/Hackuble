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
        private int count = 0;
        private float topZOrder = 0.00f;
        private Controls.OpenGLControl openGLControl1;
        private StringBuilder statusBuilder;
        public VisualScriptingEnv()
        {
            InitializeComponent();
            statusBuilder = new StringBuilder();
        }

        private void VisualScriptingEnv_Load(object sender, EventArgs e)
        {
            LoadVSEControl();
        }

        private void LoadVSEControl()
        {
            this.openGLControl1 = new Win.Controls.OpenGLControl((this.ClientSize.Width), (this.ClientSize.Height - (this.toolStrip1.Height + statusStrip1.Height)));
            this.openGLControl1.SuspendLayout();
            this.SuspendLayout();
            this.openGLControl1.Location = new System.Drawing.Point(0, this.toolStrip1.Height);
            this.openGLControl1.Name = "openGLControl1";
            this.openGLControl1.TabIndex = 3;
            this.openGLControl1.Paint += OpenGLControl1_Paint;
            this.Controls.Add(this.openGLControl1);
            this.openGLControl1.BringToFront();
            this.statusStrip1.SendToBack();
            this.openGLControl1.ResumeLayout(false);
            this.openGLControl1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout(this.openGLControl1, "Size");
        }

        private void OpenGLControl1_Paint(object sender, PaintEventArgs e)
        {
            AddInfoToStatus("FPS: " + this.openGLControl1.FrameRate.ToString());
            if (openGLControl1.ElementInFocus != null)
            {
                AddInfoToStatus("Selected Element Type: " + openGLControl1.ElementInFocus.ElementType);
                AddInfoToStatus("Element ZDepth: " + openGLControl1.ElementInFocus.ZDepth.ToString());
                AddInfoToStatus("Element ZOrder: " + openGLControl1.ElementInFocus.ZOrder.ToString());
            }
            PushStatus();
        }

        private void VisualScriptingEnv_Paint(object sender, PaintEventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            this.openGLControl1.Refresh();
        }

        private void VisualScriptingEnv_Resize(object sender, EventArgs e)
        {
            if (this.openGLControl1 != null)
            {
                this.openGLControl1.Size = new System.Drawing.Size((this.ClientSize.Width), (this.ClientSize.Height - (this.toolStrip1.Height + statusStrip1.Height)));
                this.openGLControl1.BringToFront();
            }
        }

        private void VisualScriptingEnv_ResizeEnd(object sender, EventArgs e)
        {
            this.openGLControl1.resizeOpenGL(e);
        }

        private void VisualScriptingEnv_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.openGLControl1.closeOpenGL();
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            count++;
            VisualScripting.TestComp comp = new VisualScripting.TestComp();
            comp.Name = $"Comp {count}";
            comp.Color = Color.Green;
            comp.ZOrder = topZOrder + 0.00001f;
            topZOrder += comp.ZDepth + 0.00001f;

            openGLControl1.AddComponent(comp);
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            count++;
            VisualScripting.TestComp comp = new VisualScripting.TestComp();
            comp.Name = $"Comp {count}";
            comp.Color = Color.OrangeRed;
            comp.ZOrder = topZOrder + 0.00001f;
            topZOrder += comp.ZDepth + 0.00001f;

            openGLControl1.AddComponent(comp);
        }

        private void PushStatus()
        {
            toolStripStatusLabel1.Text = statusBuilder.ToString();
            ClearStatus();
        }

        private void AddInfoToStatus(string info)
        {
            if (statusBuilder.Length > 0)
            {
                statusBuilder.Append(" | ");
            }
            statusBuilder.Append(info);
        }

        private void ClearStatus()
        {
            statusBuilder = new StringBuilder();
        }
    }
}
