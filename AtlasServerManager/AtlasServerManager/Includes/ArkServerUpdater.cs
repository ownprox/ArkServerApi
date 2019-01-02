using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Text.RegularExpressions;
using System.Threading;

namespace AtlasServerManager.Includes
{
    class ArkServerUpdater
    {
        private static bool UpdateError = false, SkipAnnounce = false, FirstLaunch = true;

        public static void CheckForUpdates(object Data)
        {
            AtlasServerManager ArkMgr = (AtlasServerManager)Data;
            ArkMgr.Log("[Atlas] Checking for updates, can take up to 30 seconds...");
            string CurrentVersion = "";
            int UpdateVersion = 0, CurrentVer = 0;
            while (true)
            {
                if (ArkMgr.checkAutoServerUpdate.Checked)
                {
                    UpdateVersion = GetAtlasServerBuildID(ArkMgr);
                    if (UpdateVersion != 0)
                    {
                        if (File.Exists(ArkMgr.SteamPath + "AtlasLatestVersion.txt")) using (StreamReader r = new StreamReader(ArkMgr.SteamPath + "AtlasLatestVersion.txt")) CurrentVersion = r.ReadLine();
                        int.TryParse(CurrentVersion, out CurrentVer);
                        if (CurrentVer != UpdateVersion)
                        {
                            ArkMgr.Updating = true;
                            ArkMgr.Log("BuildID " + UpdateVersion + " Released!");
                            if (!SkipAnnounce && Process.GetProcessesByName("ShooterGameServer").Length > 0)
                            {
                                ArkMgr.Log("[Atlas] Update Broadcasting 30 Minutes");
                                BroadCastMessage("Atlas To Version: " + UpdateVersion + ", in 30 Minutes!");
                                Thread.Sleep(900000);
                                ArkMgr.Log("[Atlas] Update Broadcasting 15 Minutes");
                                BroadCastMessage("Atlas Updating To Version: " + UpdateVersion + ", in 15 Minutes!");
                                Thread.Sleep(300000);
                                ArkMgr.Log("[Atlas] Update Broadcasting 10 Minutes");
                                BroadCastMessage("Atlas Updating To Version: " + UpdateVersion + ", in 10 Minutes!");
                                Thread.Sleep(300000);
                                ArkMgr.Log("[Atlas] Update Broadcasting 5 Minutes");
                                BroadCastMessage("Atlas Updating To Version: " + UpdateVersion + ", in 5 Minutes!");
                                Thread.Sleep(180000);
                                ArkMgr.Log("[Atlas] Update Broadcasting 2 Minutes");
                                BroadCastMessage("Atlas Updating To Version: " + UpdateVersion + ", in 2 Minutes!");
                                Thread.Sleep(60000);
                                ArkMgr.Log("[Atlas] Update Broadcasting 1 Minute");
                                BroadCastMessage("Atlas Updating To Version: " + UpdateVersion + ", in 1 Minute!");
                                Thread.Sleep(30000);
                                ArkMgr.Log("[Atlas] Update Broadcasting 30 Seconds");
                                BroadCastMessage("Atlas Updating To Version: " + UpdateVersion + ", in 30 Seconds!");
                                Thread.Sleep(30000);
                                ArkMgr.Log("[Atlas] Update Saving World");
                                BroadCastMessage("Atlas Updating To Version: " + UpdateVersion + "!, Please relaunch and update your games.");
                                Thread.Sleep(5000);
                                if (!SaveWorld())
                                {
                                    ArkMgr.Log("[Atlas] Failed Saving World, Not Updating!");
                                    continue;
                                }
                            }
                            bool ServerStillOpen = true;
                            while (ServerStillOpen)
                            {
                                Thread.Sleep(1000);
                                ServerStillOpen = Process.GetProcessesByName("ShooterGameServer").Length > 0;
                                if (!ServerStillOpen) break;
                            }
                            ArkMgr.Log("[Atlas] Current BuildID: " + CurrentVersion + ", Updating To BuildID: " + UpdateVersion);
                            UpdateArk(ArkMgr, UpdateVersion.ToString());
                            if (UpdateError)
                            {
                                ArkMgr.Log("[Atlas] Update Error, Retrying...");
                                UpdateError = false;
                                continue;
                            }
                            ArkMgr.Log("[Atlas] Updated, Launching Servers!");
                            FirstLaunch = ArkMgr.Updating = false;
                            StartServers(ArkMgr);
                        }
                        else ArkMgr.Updating = false;
                    }
                }
                else ArkMgr.Updating = false;
                if(!FirstLaunch)
                {
                    ArkMgr.Log("[Atlas] No updates found, Booting servers!");
                    FirstLaunch = false;
                    StartServers(ArkMgr);
                }
                Thread.Sleep((int)(ArkMgr.numServerUpdate.Value * 60000));
            }
        }

        private static int GetAtlasServerBuildID(AtlasServerManager ArkMgr)
        {
            try
            {
                if (!Directory.Exists(ArkMgr.SteamPath)) Directory.CreateDirectory(ArkMgr.SteamPath);
                if (!File.Exists(ArkMgr.SteamPath + "steamcmd.exe")) File.WriteAllBytes(ArkMgr.SteamPath + "steamcmd.exe", Properties.Resources.steamcmd);
                ArkMgr.UpdateProcess = new Process()
                {
                    StartInfo = new ProcessStartInfo(ArkMgr.SteamPath + "steamcmd.exe", "+@NoPromptForPassword 1 +@sSteamCmdForcePlatformType windows +login anonymous +app_info_update 1 +app_info_print 1006030 +app_info_print 1006030 +app_info_print 1006030 +quit")
                    {
                        UseShellExecute = false,
                        RedirectStandardOutput = true,
                        CreateNoWindow = true,
                        WorkingDirectory = ArkMgr.SteamPath
                    }
                };
                ArkMgr.UpdateProcess.Start();
                string output = ArkMgr.UpdateProcess.StandardOutput.ReadToEnd();
                ArkMgr.UpdateProcess.WaitForExit();
                int Version = 0;
                int BuildIndex = output.IndexOf("\"buildid\"		\"");
                if(BuildIndex != -1)
                {
                    BuildIndex += 12;
                    int EndIndex = output.IndexOf('"', BuildIndex) - BuildIndex;
                    if (EndIndex != -1) int.TryParse(output.Substring(BuildIndex, EndIndex), out Version);
                }
                return Version;
            }
            catch { ArkMgr.Log("[Update] Failed Checking For Updates..."); }     
            return 0;
        }

        private static List<string> UpdatePaths = new List<string>();
        private static void UpdateArk(AtlasServerManager ArkMgr, string UpdateVersion)
        {
            ArkMgr.Invoke((System.Windows.Forms.MethodInvoker)delegate ()
            {
                foreach (ArkServerListViewItem ASLVI in ArkMgr.ServerList.Items)
                {
                    string UpdatePath = ASLVI.GetServerData().ServerPath;
                    if (UpdatePath.StartsWith("./") || UpdatePath.StartsWith(@".\")) UpdatePath = UpdatePath.Replace("./", System.AppDomain.CurrentDomain.BaseDirectory).Replace(@".\", System.AppDomain.CurrentDomain.BaseDirectory).Replace("//", "/").Replace(@"\\", @"\");
                    
                    if (!Directory.Exists(Path.GetDirectoryName(UpdatePath))) Directory.CreateDirectory(Path.GetDirectoryName(UpdatePath));

                    if (UpdatePath.Contains(@"ShooterGame\Binaries\Win64")) UpdatePath = Regex.Split(UpdatePath, "\\ShooterGame")[0];

                    if (!UpdatePaths.Contains(UpdatePath.TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar, '\\'))) UpdatePaths.Add(UpdatePath.TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar, '\\'));
                }
            });
            
            foreach (string UpdatePath in UpdatePaths)
            {
                ArkMgr.Log("[Ark] Updating Path: " + UpdatePath);
                ArkMgr.UpdateProcess = new Process()
                {
                    StartInfo = new ProcessStartInfo(ArkMgr.SteamPath + "steamcmd.exe", "+@NoPromptForPassword 1 +@sSteamCmdForcePlatformType windows +login anonymous +force_install_dir \"" + UpdatePath + "\" +app_update 376030 validate +quit")
                    {
                        UseShellExecute = false,
                        RedirectStandardInput = true,
                        RedirectStandardOutput = true,
                        CreateNoWindow = true,
                        WorkingDirectory = ArkMgr.SteamPath
                    }
                };
                ArkMgr.UpdateProcess.OutputDataReceived += (s, e) => UpdateData(e.Data);
                ArkMgr.UpdateProcess.Start();
                ArkMgr.UpdateProcess.BeginOutputReadLine();
                ArkMgr.UpdateProcess.WaitForExit();
            }
            if (!UpdateError) using (StreamWriter w = new StreamWriter(ArkMgr.SteamPath + "AtlasLatestVersion.txt")) w.WriteLine(UpdateVersion);
        }

        private static void StartServers(AtlasServerManager ArkMgr)
        {
            ArkMgr.Invoke((System.Windows.Forms.MethodInvoker)delegate ()
            {
                foreach (ArkServerListViewItem ASLVI in ArkMgr.ServerList.Items)
                {
                    ASLVI.GetServerData().StartServer();
                }
            });
        }

        private static void UpdateData(string input)
        {
            if (input != null && input.Length > 1)
            {
                if (input.Contains("Error! App ")) UpdateError = true;
                AtlasServerManager.GetInstance().Log("[Update]" + input);
            }
        }

        static bool ConnectToRcon(SourceRcon Sr, string IP, int Port, string Pass)
        {
            if (Sr != null && !Sr.Connected)
            {
                int DotCount = 0;
                for (int i = 0; i < IP.Length; i++) if (IP[i] == '.') DotCount++;
                if (DotCount != 3)
                {
                    IPAddress[] ips = Dns.GetHostAddresses(IP);
                    if (ips.Length > 0) IP = ips[0].ToString();
                }

                bool Connect = Sr.Connect(new IPEndPoint(IPAddress.Parse(IP), Port), Pass);
                int Counter = 0;
                while (!Sr.Connected)
                {
                    Thread.Sleep(1000);
                    if (Counter++ > 30)
                    {
                        AtlasServerManager.GetInstance().Log("[Rcon->ConnectToRcon] Something is wrong with connection");
                        return false;
                    }
                    else if (Sr.Connected) break;
                }
            }
            return true;
        }

        static bool BroadCastMessage(string Message)
        {
            try
            {
                bool FirstRun = true;
                AtlasServerManager.GetInstance().Invoke((System.Windows.Forms.MethodInvoker)delegate ()
                {
                    foreach (ArkServerListViewItem ASLVI in AtlasServerManager.GetInstance().ServerList.Items)
                    {
                        if (!FirstRun) Thread.Sleep(4000);
                        if (!ConnectToRcon(ASLVI.GetServerData().RconConnection, ASLVI.GetServerData().ServerIp, ASLVI.GetServerData().RconPort, ASLVI.GetServerData().Pass)) continue;
                        ASLVI.GetServerData().RconConnection.ServerCommand("broadcast " + Message);
                        FirstRun = false;
                    }
                });
                return true;
            }
            catch (Exception e)
            {
                AtlasServerManager.GetInstance().Log("[Rcon->BroadCastMessage] Connection failed: " + e.Message);
                return false;
            }
        }

        static bool SaveWorld()
        {
            bool FirstRun = true;
            int FailCount = 0;
            AtlasServerManager.GetInstance().Invoke((System.Windows.Forms.MethodInvoker)delegate ()
            {
                foreach (ArkServerListViewItem ASLVI in AtlasServerManager.GetInstance().ServerList.Items)
                {
                    if (!FirstRun) Thread.Sleep(4000);
                    if (!ConnectToRcon(ASLVI.GetServerData().RconConnection, ASLVI.GetServerData().ServerIp, ASLVI.GetServerData().RconPort, ASLVI.GetServerData().Pass))
                    {
                        FailCount++;
                        continue;
                    }
                    ASLVI.GetServerData().RconConnection.ServerCommand("DoExit");
                    FirstRun = false;
                }
            });
            return FailCount != AtlasServerManager.GetInstance().ServerList.Items.Count;
        }
    }
}