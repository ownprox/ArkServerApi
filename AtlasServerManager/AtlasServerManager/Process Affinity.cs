using System;
using System.Drawing;
using System.Windows.Forms;

namespace AtlasServerManager
{
    public partial class Process_Affinity : Form
    {
        bool HasLoaded = false;
        private int StartControlIndex = 0;
        public bool[] ProcessAffinity;

        public Process_Affinity()
        {
            InitializeComponent();
            StartControlIndex = Controls.Count;
            DrawCheckboxs();
        }

        public void DrawCheckboxs()
        {
            if (HasLoaded) return;
            HasLoaded = true;
            int X = 10, Y = 10;
            for (int i = 0; i < Environment.ProcessorCount; i++)
            {
                CheckBox chkbox = new CheckBox();
                chkbox.Text = chkbox.Name = "CPU " + i;
                chkbox.Location = new Point(X, Y);
                X += 80;
                if(X > (Width - 80))
                {
                    X = 10;
                    Y += 20;
                }
                chkbox.Size = new Size(70, 20);
                chkbox.Checked = true;
                Controls.Add(chkbox);
            }
        }

        public void UpdateCheckBoxs(bool[] PAPA)
        {
            int Chkboxindex = StartControlIndex;
            for (int i = 0; i < PAPA.Length; i++)
            {
                if (Chkboxindex >= Controls.Count) break;
                ((CheckBox)Controls[Chkboxindex++]).Checked = PAPA[i];
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            ProcessAffinity = new bool[Environment.ProcessorCount];
            for(int i = StartControlIndex, j = 0; i < Environment.ProcessorCount; i++, j++)
                ProcessAffinity[j] = ((CheckBox)Controls[i]).Checked;
        }
    }
}