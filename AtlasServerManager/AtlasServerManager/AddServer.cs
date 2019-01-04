using System;
using System.Windows.Forms;
using AtlasServerManager.Includes;

namespace AtlasServerManager
{
    public partial class AddServer : Form
    {
        private readonly Process_Affinity Pa = new Process_Affinity();
        public bool Create;
        public AtlasServerData ServerData;
        private string ServerPath = string.Empty;

        public AddServer(string ServerPath)
        {
            InitializeComponent();
            Text = "Create Server";
            Create = true;
            CopyServerData();
            Registry.SaveRegServer(AtlasServerManager.GetInstance(), ServerData, 0, true);
            ServerData = Registry.LoadRegServer(AtlasServerManager.GetInstance(), "LastSaved");
            if (ServerData.Loaded)
            {
                UpdateComponents(ServerData);
            }
            else if (ServerPath != string.Empty)
            {
                filePathBox.Text = ServerPath;
                this.ServerPath = ServerPath;
            }
        }

        public AddServer(AtlasServerData ServerData, string ServerPath)
        {
            InitializeComponent();
            Create = false;
            UpdateComponents(ServerData);
        }

        private void UpdateComponents(AtlasServerData Data)
        {
            ServerData = Data;
            filePathBox.Text = Data.ServerPath;
            ServerPath = Data.ServerPath;
            textBox5.Text = Data.AltSaveDirectory;
            numericUpDown1.Value = Data.MaxPlayers;
            numericUpDown3.Value = Data.QueryPort;
            numericUpDown2.Value = Data.ServerPort;
            maskedTextBox1.Text = Data.Pass;
            checkBox1.Checked = Data.WildWipe;
            checkBox10.Checked = Data.PVP;
            checkBox11.Checked = Data.MapB;
            checkBox12.Checked = Data.Gamma;
            checkBox13.Checked = Data.Third;
            checkBox14.Checked = Data.Crosshair;
            checkBox15.Checked = Data.HitMarker;
            checkBox16.Checked = Data.Imprint;
            checkBox17.Checked = Data.FTD;
            textBox2.Text = Data.CustomArgs;
            numericUpDown5.Value = Data.ServerX;
            numericUpDown7.Value = Data.ServerY;
            numericUpDown6.Value = Data.ReservedPlayers;
            textBox1.Text = Data.ServerIp;
            comboBox1.SelectedIndex = Data.ProcessPriority;

            /*Process Affinity*/
            if (Data.ProcessAffinity == null || Data.ProcessAffinity.Length == 0)
                Data.ProcessAffinity = new bool[Environment.ProcessorCount];
            Pa.UpdateCheckBoxs(Data.ProcessAffinity);

            /*Rcon*/

            checkBox3.Checked = Data.Rcon;
            numericUpDown4.Value = Data.RconPort;
            if (!Create)
            {
                Text = "Edit: ServerX: " + Data.ServerX + ", ServerY: " + Data.ServerX + ", Port: " +
                       Data.ServerPort;
                createButton.Text = "Save Settings";
            }
        }

        private void CopyServerData()
        {
            if (filePathBox.Text == string.Empty) filePathBox.Text = @".\AtlasServerData\ShooterGame\Binaries\Win64\ShooterGameServer.exe";

            ServerData = new AtlasServerData
            {
                ServerPath = filePathBox.Text,
                AltSaveDirectory = textBox5.Text,
                MaxPlayers = (int) numericUpDown1.Value,
                QueryPort = (int) numericUpDown3.Value,
                ServerPort = (int) numericUpDown2.Value,
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
                ProcessAffinity = Pa.ProcessAffinity,
                /*Rcon*/
                Rcon = checkBox3.Checked,
                RconPort = (int) numericUpDown4.Value,
                AutoStart = true,
                ServerX = (int) numericUpDown5.Value,
                ServerY = (int) numericUpDown7.Value,
                ReservedPlayers = (int) numericUpDown6.Value,
                ServerIp = textBox1.Text,
                ProcessPriority = comboBox1.SelectedIndex
            };
        }

        private void CreateServer_Click(object sender, EventArgs e)
        {
            string Error = string.Empty;
            foreach (AtlasServerData Data in AtlasServerManager.GetInstance().ServerList.GetServerList())
            {
                if (Data.ServerPort == ServerData.ServerPort && Data.QueryPort == ServerData.QueryPort &&
                    Data.RconPort == ServerData.RconPort) continue;
                if (Data.ServerPort == (int) numericUpDown2.Value)
                {
                    Error = "Server Port " + Data.ServerPort + " is already in use by ServerX: " + ServerData.ServerX +
                            ", ServerY: " + ServerData.ServerX + ", Port: " + ServerData.ServerPort +
                            ", Please change it";
                    break;
                }

                if (Data.QueryPort == (int) numericUpDown3.Value)
                {
                    Error = "Query Port " + Data.QueryPort + " is already in use by " + ServerData.ServerX +
                            ", ServerY: " + ServerData.ServerX + ", Port: " + ServerData.ServerPort +
                            ", Please change it";
                    break;
                }

                if (Data.RconPort == (int) numericUpDown4.Value)
                {
                    Error = "Rcon Port " + Data.RconPort + " is already in use by " + ServerData.ServerX +
                            ", ServerY: " + ServerData.ServerX + ", Port: " + ServerData.ServerPort +
                            ", Please change it";
                    break;
                }
            }

            if (textBox1.Text == string.Empty || !textBox1.Text.Contains(".")) Error = "Please set a Server IP!";
            if (textBox5.Text == string.Empty) Error = "Please set a Alt Save Directory!";

            if (Error != string.Empty)
            {
                MessageBox.Show(Error);
            }
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
            if (!checkBox3.Checked &&
                MessageBox.Show("Disabling this will disable auto updates, Are you sure you want to disable this?",
                    "Warning !!!", MessageBoxButtons.YesNo, MessageBoxIcon.Information) ==
                DialogResult.No) checkBox3.Checked = true;
        }

        private void AddServer_Load(object sender, EventArgs e)
        {
            Pa.DrawCheckboxs();
        }

        private void CreateButton_Click(object sender, EventArgs e)
        {
            ServerData = Registry.LoadRegServer(AtlasServerManager.GetInstance(), "Default");
            UpdateComponents(ServerData);
            Registry.SaveRegServer(AtlasServerManager.GetInstance(), ServerData, 0, true, true);
        }

        private void BrowseButton_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK) filePathBox.Text = openFileDialog1.FileName;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (Pa.ShowDialog() == DialogResult.OK) ServerData.ProcessAffinity = Pa.ProcessAffinity;
        }

    }
}