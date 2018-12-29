using System;
using System.IO;
using System.Windows.Forms;
using AtlasServerManager.Includes;

namespace AtlasServerManager
{
    public partial class AddServer : Form
    {
        public AtlasServerData ServerData;
        public bool Create = true;
        string ServerPath = string.Empty;
        Process_Affinity PA = new Process_Affinity();

        public AddServer(string ServerPath)
        {
            InitializeComponent();
            Text = "Create Server";
            Create = true;
            CopyServerData();
            Registry.SaveRegServer(AtlasServerManager.GetInstance(), ServerData, 0, true);
            ServerData = Registry.LoadRegServer(AtlasServerManager.GetInstance(), "LastSaved");
            if (ServerData.Loaded) UpdateComponents(ServerData);
            else if (ServerPath != string.Empty)
            {
                textBox4.Text = ServerPath;
                this.ServerPath = ServerPath;
            }
        }

        public AddServer(AtlasServerData ServerData, string ServerPath)
        {
            InitializeComponent();
            Create = false;
            UpdateComponents(ServerData);
        }

        private void UpdateComponents(AtlasServerData ServerData)
        {
            this.ServerData = ServerData;
            textBox4.Text = ServerData.ServerPath;
            ServerPath = ServerData.ServerPath;
            textBox5.Text = ServerData.AltSaveDirectory;
            numericUpDown1.Value = ServerData.MaxPlayers;
            numericUpDown3.Value = ServerData.QueryPort;
            numericUpDown2.Value = ServerData.ServerPort;
            maskedTextBox1.Text = ServerData.Pass;
            checkBox1.Checked = ServerData.WildWipe;
            checkBox10.Checked = ServerData.PVP;
            checkBox11.Checked = ServerData.MapB;
            checkBox12.Checked = ServerData.Gamma;
            checkBox13.Checked = ServerData.Third;
            checkBox14.Checked = ServerData.Crosshair;
            checkBox15.Checked = ServerData.HitMarker;
            checkBox16.Checked = ServerData.Imprint;
            checkBox17.Checked = ServerData.FTD;
            textBox2.Text = ServerData.CustomArgs;
            numericUpDown5.Value = ServerData.ServerX;
            numericUpDown7.Value = ServerData.ServerY;
            numericUpDown6.Value = ServerData.ReservedPlayers;
            textBox1.Text = ServerData.ServerIp;

            /*Process Affinity*/
            if (ServerData.ProcessAffinity == null || ServerData.ProcessAffinity.Length == 0)
                ServerData.ProcessAffinity = new bool[Environment.ProcessorCount];
            PA.UpdateCheckBoxs(ServerData.ProcessAffinity);

            /*Rcon*/
            
            checkBox3.Checked = ServerData.Rcon;
            numericUpDown4.Value = ServerData.RconPort;
            if (!Create)
            {
                Text = "Edit: ServerX: " + ServerData.ServerX + ", ServerY: " + ServerData.ServerX + ", Port: " + ServerData.ServerPort;
                button1.Text = "Save Settings";
            }
        }

        private void CopyServerData()
        {
            if(textBox4.Text == string.Empty)
            {
                textBox4.Text = @".\AtlasServerData";
            }

            ServerData = new AtlasServerData()
            {
                ServerPath = textBox4.Text,
                AltSaveDirectory = textBox5.Text,
                MaxPlayers = (int)numericUpDown1.Value,
                QueryPort = (int)numericUpDown3.Value,
                ServerPort = (int)numericUpDown2.Value,
                Pass = maskedTextBox1.Text,
                WildWipe = checkBox1.Checked,
                PVP = checkBox10.Checked,
                MapB = checkBox11.Checked,
                Gamma = checkBox12.Checked,
                Third = checkBox13.Checked,
                Crosshair = checkBox14.Checked,
                HitMarker = checkBox15.Checked,
                Imprint = checkBox16.Checked,
                FTD = checkBox17.Checked,
                CustomArgs = textBox2.Text,
                /*Process Affinity*/
                ProcessAffinity = PA.ProcessAffinity,
                /*Rcon*/
                Rcon = checkBox3.Checked,
                RconPort = (int)numericUpDown4.Value,
                AutoStart = true,
                ServerX = (int)numericUpDown5.Value,
                ServerY = (int)numericUpDown7.Value,
                ReservedPlayers = (int)numericUpDown6.Value,
                ServerIp = textBox1.Text
            };
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string Error = string.Empty;
            foreach (AtlasServerData Asd in AtlasServerManager.GetInstance().ServerList.GetServerList())
            {
                if (Asd.ServerPort == ServerData.ServerPort && Asd.QueryPort == ServerData.QueryPort && Asd.RconPort == ServerData.RconPort) continue;
                if (Asd.ServerPort == (int)numericUpDown2.Value)
                {
                    Error = "Server Port " + Asd.ServerPort + " is already in use by ServerX: " + ServerData.ServerX + ", ServerY: " + ServerData.ServerX + ", Port: " + ServerData.ServerPort + ", Please change it";
                    break;
                }
                if (Asd.QueryPort == (int)numericUpDown3.Value)
                {
                    Error = "Query Port " + Asd.QueryPort + " is already in use by " + ServerData.ServerX + ", ServerY: " + ServerData.ServerX + ", Port: " + ServerData.ServerPort + ", Please change it";
                    break;
                }
                if (Asd.RconPort == (int)numericUpDown4.Value)
                {
                    Error = "Rcon Port " + Asd.RconPort + " is already in use by " + ServerData.ServerX + ", ServerY: " + ServerData.ServerX + ", Port: " + ServerData.ServerPort + ", Please change it";
                    break;
                }
            }

            if(textBox1.Text == string.Empty || !textBox1.Text.Contains("."))
            {
                Error = "Please set a Server IP!";
            }

            if (textBox5.Text == string.Empty)
            {
                Error = "Please set a Alt Save Directory!";
            }


            if (Error != string.Empty) MessageBox.Show(Error);
            else
            {
                CopyServerData();
                Registry.SaveRegServer(AtlasServerManager.GetInstance(), ServerData, 0, true, true);
                DialogResult = DialogResult.OK;
                Close();
            }
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (!checkBox3.Checked && MessageBox.Show("Disabling this will disable auto updates, Are you sure you want to disable this?", "Warning !!!", MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.No) checkBox3.Checked = true;
        }

        private void AddServer_Load(object sender, EventArgs e)
        {
            PA.DrawCheckboxs();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            ServerData = Registry.LoadRegServer(AtlasServerManager.GetInstance(), "Default");
            UpdateComponents(ServerData);
            Registry.SaveRegServer(AtlasServerManager.GetInstance(), ServerData, 0, true, true);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (ServerPath != string.Empty) folderBrowserDialog1.RootFolder = Environment.SpecialFolder.MyComputer;
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK && Directory.Exists(folderBrowserDialog1.SelectedPath)) textBox4.Text = folderBrowserDialog1.SelectedPath;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if(PA.ShowDialog() == DialogResult.OK)
            {
                ServerData.ProcessAffinity = PA.ProcessAffinity;
            }
        }
    }
}