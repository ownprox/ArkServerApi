using System;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;

namespace AtlasServerManager.Includes
{
    public class AtlasServerData
    {
        public SourceRcon RconConnection = new SourceRcon();
        public Process ServerProcess;
        public string Pass = "", CustomArgs = "", ServerPath = "", FinalServerPath = "", AltSaveDirectory = "", ServerIp = "", RCONIP = "";
        public int ServerPort, QueryPort, RconPort, MaxPlayers, ReservedPlayers, ServerX, ServerY, PID = 0, ProcessPriority;
        public bool[] ProcessAffinity;
        public bool Rcon, FTD, WildWipe, PVP, MapB, Gamma, Third, Crosshair, HitMarker, Imprint, Loaded, AutoStart;
        private bool HasMadeFirstContact, AttemptRconSave = false, GamePortWasOpen = false;
        private DateTime LastSourceQueryReply, RconSavedEstimate;
        public void InitStartServer()
        {
            string ArkManagerPath = AtlasServerManager.GetInstance().ArkManagerPath;
            FinalServerPath = ServerPath;
            if (FinalServerPath[0] == '.' && (FinalServerPath[1] == '\\' || FinalServerPath[1] == '/'))
            {
                string[] BaseFolder = FinalServerPath.Replace(".\\", "").Replace("./", "").Split('/');
                if (BaseFolder.Length == 1) BaseFolder = BaseFolder[0].Split('\\');
                if (BaseFolder.Length > 0)
                {
                    if (!Directory.Exists(ArkManagerPath + BaseFolder[0])) Directory.CreateDirectory(ArkManagerPath + BaseFolder[0]);
                    FinalServerPath = FinalServerPath.Replace(".\\", ArkManagerPath).Replace("./", ArkManagerPath);
                }
                else FinalServerPath = ArkManagerPath + @".\AtlasServerData\";
            }
            else if (!File.Exists(FinalServerPath)) MessageBox.Show(FinalServerPath + " Path not found!");
        }

        public void StartServer()
        {
            if (FinalServerPath == string.Empty) InitStartServer();
            HasMadeFirstContact = false;
            string ExePath = ServerPath;
            if (ExePath.StartsWith("./") || ExePath.StartsWith(@".\"))
                ExePath = (ExePath.Replace("./", System.AppDomain.CurrentDomain.BaseDirectory).Replace(@".\", System.AppDomain.CurrentDomain.BaseDirectory) + "\\").Replace("//", "/").Replace(@"\\", @"\");

            if (!ExePath.ToLower().Contains(@"\shootergame\binaries\win64\"))
                ExePath = Path.Combine(ExePath, "ShooterGame\\Binaries\\Win64\\");
            if (!ExePath.ToLower().EndsWith("shootergameserver.exe"))
                ExePath = Path.Combine(ExePath, "ShooterGameServer.exe");
            if (!File.Exists(ExePath))
            {
                MessageBox.Show(ExePath + "  Is Not found!!!", "ShooterGameServer.exe Not Found!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (IsRunning()) StopServer();

            GamePortWasOpen = false;
            try
            {
                ServerProcess = new Process
                {
                    StartInfo = new ProcessStartInfo(ExePath, "\"" + "Ocean?ServerX=" + ServerX + "?ServerY=" + ServerY + "?Port=" + ServerPort + "?QueryPort=" + QueryPort + "?AltSaveDirectoryName=" + AltSaveDirectory + "?MaxPlayers=" + MaxPlayers + "?ReservedPlayerSlots=" + ReservedPlayers + "?ServerAdminPassword=" + Pass + "?ServerCrosshair=" + (Crosshair ? "true" : "false") + "?AllowThirdPersonPlayer=" + (Third ? "true" : "false") + "?MapPlayerLocation=" + (MapB ? "true" : "false") + "?serverPVE=" + (!PVP ? "true" : "false") + "?RCONEnabled=" + (Rcon ? ("true?RCONPort=" + RconPort) : "false") + "?EnablePvPGamma=" + (Gamma ? "true" : "false") + "?AllowAnyoneBabyImprintCuddle=" + (Imprint ? "true" : "false") + "?ShowFloatingDamageText=" + FTD + "?SeamlessIP=" + ServerIp + CustomArgs + "\" -game -server -log -NoBattlEye -NoCrashDialog")
                    {
                        UseShellExecute = false,
                        WorkingDirectory = Path.GetDirectoryName(ExePath)
                    }
                };

                ServerProcess.Start();
                int AffinityMask = 0;
                for (int i = 0; i < ProcessAffinity.Length; i++) AffinityMask |= (ProcessAffinity[i] ? 1 : 0) << i;
                ServerProcess.ProcessorAffinity = (System.IntPtr)AffinityMask;
                switch(ProcessPriority)
                {
                    case 1:
                        ServerProcess.PriorityClass = ProcessPriorityClass.AboveNormal;
                        break;
                    case 2:
                        ServerProcess.PriorityClass = ProcessPriorityClass.High;
                        break;
                    case 3:
                        ServerProcess.PriorityClass = ProcessPriorityClass.RealTime;
                        break;
                    default:
                        ServerProcess.PriorityClass = ProcessPriorityClass.Normal;
                        break;

                }
                PID = ServerProcess.Id;
            }
            catch (Exception e)
            {
                ServerProcess = null;
                MessageBox.Show(e.Message + ": " + ExePath, "Error");
            }
        }

        public void GetPlayersOnline(AtlasServerManager ArkMgr, ArkServerListViewItem server)
        {
            new SourceQuery(ServerIp, QueryPort, (SourceQuery.ServerData SrvData) =>
            {
                ArkMgr.Invoke((MethodInvoker)delegate ()
                {
                    if (server != null)
                    {
                        server.SubItems[5].Text = SrvData.Players.ToString();
                        HasMadeFirstContact = true;
                        LastSourceQueryReply = DateTime.Now;
                    }
                });
            }).Dispose();
        }

        private bool CheckGamePort()
        {
            bool IsConnected = false;
            try
            {
                System.Net.Sockets.Socket sock = new System.Net.Sockets.Socket(System.Net.Sockets.AddressFamily.InterNetwork, System.Net.Sockets.SocketType.Dgram, System.Net.Sockets.ProtocolType.Udp);
                sock.Connect(ServerIp, ServerPort);
                IsConnected = sock.Connected;
                sock.Shutdown(System.Net.Sockets.SocketShutdown.Both);
                sock.Close();
            } catch {}
            if (!GamePortWasOpen && IsConnected) GamePortWasOpen = true;
            return IsConnected;
        }

        public int IsRunning(AtlasServerManager ArkMgr)
        {
            if (AttemptRconSave && DateTime.Now.Subtract(RconSavedEstimate).TotalSeconds <= 0) return 1;
            if (ServerProcess != null && (ServerProcess.HasExited || ServerProcess.Id == 0)) return 0;
            if (HasMadeFirstContact && ArkMgr != null)
            {
                if (ArkMgr.QueryPortCheck.Checked && DateTime.Now.Subtract(LastSourceQueryReply).TotalSeconds > 60) return 2;
                if (ArkMgr.GamePortCheck.Checked && GamePortWasOpen && !CheckGamePort())
                {
                    if(!AttemptRconSave)
                    {
                        AttemptRconSave = true;
                        SourceRconTools.SendCommand("DoExit", this);
                        RconSavedEstimate = DateTime.Now.AddMinutes(1);
                        return 4;
                    }
                    AttemptRconSave = false;
                    return 3;
                }
            }
            return ServerProcess == null ? 0 : 1;
        }

        public bool IsRunning() { return IsRunning(null) == 1; }

        public void StopServer()
        {
            HasMadeFirstContact = false;
            if (IsRunning())
            {
                ServerProcess.Kill();
                ServerProcess = null;
            }
        }
    };
}