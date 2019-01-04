using System;
using System.Windows.Forms;
using AtlasServerManager.Includes;
using System.IO;
using System.Diagnostics;

namespace AtlasServerManager
{
    public partial class AtlasServerManager : Form
    {
        public string SteamPath = System.AppDomain.CurrentDomain.BaseDirectory + @"Steam\";
        public string ArkManagerPath = "", ServerPath = string.Empty;
        private static AtlasServerManager instance;
        public static AtlasServerManager GetInstance() { return instance; }
        public bool Updating = true;
        public Process UpdateProcess = null;

        public AtlasServerManager()
        {
            InitializeComponent();
            instance = this;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ArkManagerPath = (Application.StartupPath + @"\").Replace(@"\\", @"\");
            Worker.Start(this);
            Registry.LoadRegConfig(this);
            if (File.Exists(ArkManagerPath + "ShooterGameServer.exe")) ServerPath = ArkManagerPath + "ShooterGameServer.exe";
            else
            {
                string[] Files = Directory.GetFiles(ArkManagerPath, "*.exe", SearchOption.AllDirectories);
                foreach(string file in Files)
                {
                    if (Path.GetFileNameWithoutExtension(file) == "ShooterGameServer") ServerPath = file;
                }
            }

            if (!checkAutoServerUpdate.Checked) Updating = false;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Worker.Destroy();
            Registry.SaveRegConfig(this);
            if (UpdateProcess != null && !UpdateProcess.HasExited && UpdateProcess.Id != 0) UpdateProcess.Kill();
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddServer AddSrv = new AddServer(ServerPath);
            if (AddSrv.ShowDialog() == DialogResult.OK) ServerList.Items.Add(new ArkServerListViewItem(AddSrv.ServerData));
            AddSrv.Dispose();
        }

        private void removeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ServerList.FocusedItem != null && MessageBox.Show("Are you sure you want to delete ServerX:" + ((ArkServerListViewItem)ServerList.FocusedItem).GetServerData().ServerX + ", ServerY: " + ((ArkServerListViewItem)ServerList.FocusedItem).GetServerData().ServerY + ", Port: " + ((ArkServerListViewItem)ServerList.FocusedItem).GetServerData().ServerPort + "?\n Press 'Yes' To Delete!", "Delete ServerX:" + ((ArkServerListViewItem)ServerList.FocusedItem).GetServerData().ServerX + ", ServerY: " + ((ArkServerListViewItem)ServerList.FocusedItem).GetServerData().ServerY + ", Port: " + ((ArkServerListViewItem)ServerList.FocusedItem).GetServerData().ServerPort + "?", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                Registry.DeleteServer(ServerList.FocusedItem.Index);
                ServerList.Items.RemoveAt(ServerList.FocusedItem.Index);
            }
        }

        private void editSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ServerList.FocusedItem != null)
            {
                ArkServerListViewItem ASLVI = ((ArkServerListViewItem)ServerList.FocusedItem);
                AddServer AddSrv = new AddServer(ASLVI.GetServerData(), ServerPath);
                if (AddSrv.ShowDialog() == DialogResult.OK) ASLVI.SetServerData(AddSrv.ServerData);
                AddSrv.Dispose();
            }
        }

        private void startToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ServerList.FocusedItem != null)
            {
                ArkServerListViewItem SelectedServer = ((ArkServerListViewItem)ServerList.FocusedItem);
                if (SelectedServer.GetServerData().FinalServerPath == string.Empty) SelectedServer.GetServerData().InitStartServer(this);
                bool IsServerRunning = false;
                foreach (ArkServerListViewItem ASD in ServerList.Items) if (ASD.GetServerData().IsRunning()) IsServerRunning = true;
                //Check For Updates
                SelectedServer.GetServerData().StartServer();
            }
        }

        private void stopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ServerList.FocusedItem != null)
            {
                ArkServerListViewItem ASLVI = (ArkServerListViewItem)ServerList.FocusedItem;
                ASLVI.GetServerData().StopServer();
                ASLVI.UpdateStatus();
            }
        }

        private delegate void RichTextBoxUpdateEventHandler(string txt);

        private void checkAutoServerUpdate_CheckedChanged(object sender, EventArgs e)
        {
            if(!checkAutoServerUpdate.Checked)
            Updating = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(Updating)
            {
                MessageBox.Show("Already Updating", "Update in progress", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            if (File.Exists(SteamPath + "AtlasLatestVersion.txt")) File.Delete(SteamPath + "AtlasLatestVersion.txt");
            Log("[Atlas] Forcing Update");
            Worker.DestroyAndRecreateThread(this, Worker.WorkerType.ServerUpdateCheck);
        }

        private void ServerList_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        public void Log(string txt)
        {
            if (richTextBox1.InvokeRequired)
            {
                richTextBox1.Invoke(new RichTextBoxUpdateEventHandler(Log), new object[] { txt });
            }
            else
            {

                if (txt.Contains("downloading") && richTextBox1.Lines[richTextBox1.Lines.Length - 1].Contains("downloading"))
                {
                    richTextBox1.Lines[richTextBox1.Lines.Length - 1] = txt;
                }
                else
                {
                    richTextBox1.AppendText(string.Format("[{0}] {1}", DateTime.Now.ToString("hh:mm"), txt) + Environment.NewLine);
                    richTextBox1.ScrollToCaret();
                }
            }
        }
    }
}