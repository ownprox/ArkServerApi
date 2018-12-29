using System;
using System.Diagnostics;
using System.Text.RegularExpressions;

namespace AtlasServerManager.Includes
{
    public class Registry
    {
        private static Microsoft.Win32.RegistryKey key;

        public static void LoadRegConfig(AtlasServerManager ArkMgr)
        {
            try
            {
                key = Microsoft.Win32.Registry.CurrentUser.OpenSubKey("SOFTWARE\\AtlasServerManager", true);
                if (key == null) key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey("SOFTWARE\\AtlasServerManager");

                if (key != null)
                {
                    /* BOOL */
                    ArkMgr.checkAutoServerUpdate.Checked = (int)key.GetValue("AutoServerUpdate", ArkMgr.checkAutoServerUpdate.Checked ? 1 : 0) == 1 ? true : false;
                    ArkMgr.checkAutoModUpdate.Checked = (int)key.GetValue("AutoModUpdate", ArkMgr.checkAutoModUpdate.Checked ? 1 : 0) == 1 ? true : false;
                    ArkMgr.checkBootWhenOff.Checked = (int)key.GetValue("BootWhenOff", ArkMgr.checkBootWhenOff.Checked ? 1 : 0) == 1 ? true : false;

                    /* DECIMAL */
                    decimal value = 1.0M;
                    decimal.TryParse((string)key.GetValue("ServerUpdate", ArkMgr.numServerUpdate.Value.ToString()), out value);
                    ArkMgr.numServerUpdate.Value = value;
                    decimal.TryParse((string)key.GetValue("ServerWarning", ArkMgr.numServerWarning.Value.ToString()), out value);
                    ArkMgr.numServerWarning.Value = value;
                    decimal.TryParse((string)key.GetValue("AutoUpdateMod", ArkMgr.numAutoUpdateMod.Value.ToString()), out value);
                    ArkMgr.numAutoUpdateMod.Value = value;
                    decimal.TryParse((string)key.GetValue("AutoUpdateModWarning", ArkMgr.numAutoUpdateModWarning.Value.ToString()), out value);
                    ArkMgr.numAutoUpdateModWarning.Value = value;
                    decimal.TryParse((string)key.GetValue("ServerMonitor", ArkMgr.numServerMonitor.Value.ToString()), out value);
                    ArkMgr.numServerMonitor.Value = value;

                    /* STRING */
                    ArkMgr.ServerPath = (string)key.GetValue("ServerDataPath", string.Empty);

                    LoadRegServers(ArkMgr);
                }
            }
            catch (Exception e) { System.Windows.Forms.MessageBox.Show("Failed To Load Setting: " + e.Message); }
        }

        public static void SaveRegConfig(AtlasServerManager ArkMgr)
        {
            try
            {
                key = Microsoft.Win32.Registry.CurrentUser.OpenSubKey("SOFTWARE\\AtlasServerManager", true);
                if (key != null)
                {
                    /* BOOL */
                    key.SetValue("AutoServerUpdate", ArkMgr.checkAutoServerUpdate.Checked ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                    key.SetValue("AutoModUpdate", ArkMgr.checkAutoModUpdate.Checked ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                    key.SetValue("BootWhenOff", ArkMgr.checkBootWhenOff.Checked ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);

                    /* DECIMAL */
                    key.SetValue("ServerUpdate", ArkMgr.numServerUpdate.Value.ToString(), Microsoft.Win32.RegistryValueKind.String);
                    key.SetValue("ServerWarning", ArkMgr.numServerWarning.Value.ToString(), Microsoft.Win32.RegistryValueKind.String);
                    key.SetValue("AutoUpdateMod", ArkMgr.numAutoUpdateMod.Value.ToString(), Microsoft.Win32.RegistryValueKind.String);
                    key.SetValue("AutoUpdateModWarning", ArkMgr.numAutoUpdateModWarning.Value.ToString(), Microsoft.Win32.RegistryValueKind.String);
                    key.SetValue("ServerMonitor", ArkMgr.numServerMonitor.Value.ToString(), Microsoft.Win32.RegistryValueKind.String);

                    /* STRING */
                    key.SetValue("ServerDataPath", ArkMgr.ServerPath, Microsoft.Win32.RegistryValueKind.String);

                    SaveRegServers(ArkMgr);
                }
            }
            catch (Exception e) { System.Windows.Forms.MessageBox.Show("Failed To Save Setting: " + e.Message); }
        }

        public static void DeleteServer(int Index)
        {
            try
            {
                key = Microsoft.Win32.Registry.CurrentUser.OpenSubKey("SOFTWARE\\AtlasServerManager\\Servers", true);
                if (key != null) key.DeleteSubKey("Server" + Index);
            }
            catch (Exception e) { System.Windows.Forms.MessageBox.Show("Failed To Delete Server Setting: " + e.Message); }
        }

        private static void LoadRegServers(AtlasServerManager ArkMgr)
        {
            key = Microsoft.Win32.Registry.CurrentUser.OpenSubKey("SOFTWARE\\AtlasServerManager\\Servers");
            if (key != null)
            {
                string[] Servers = key.GetSubKeyNames();
                foreach (string Srv in Servers)
                    if (Srv.StartsWith("Server"))
                    {
                        AtlasServerData ASD = LoadRegServer(ArkMgr, Srv);
                        try
                        {
                            if (ASD.PID != 0) ASD.ServerProcess = Process.GetProcessById(ASD.PID);
                        }
                        catch { ASD.PID = 0; }
                        ArkMgr.ServerList.Items.Add(new ArkServerListViewItem(ASD));
                    }
                key.Close();
            }
        }

        public static AtlasServerData LoadRegServer(AtlasServerManager ArkMgr, string Srv)
        {
            AtlasServerData Asd = new AtlasServerData();
            key = Microsoft.Win32.Registry.CurrentUser.OpenSubKey("SOFTWARE\\AtlasServerManager\\Servers\\" + Srv);
            if (key != null)
            {
                /* BOOL */
                Asd.Rcon = (int)key.GetValue("Rcon", Asd.Rcon ? 1 : 0) == 1 ? true : false;
                Asd.WildWipe = (int)key.GetValue("WildWipe", Asd.WildWipe ? 1 : 0) == 1 ? true : false;
                Asd.PVP = (int)key.GetValue("PVP", Asd.PVP ? 1 : 0) == 1 ? true : false;
                Asd.MapB = (int)key.GetValue("MapB", Asd.MapB ? 1 : 0) == 1 ? true : false;
                Asd.Gamma = (int)key.GetValue("Gamma", Asd.Gamma ? 1 : 0) == 1 ? true : false;
                Asd.Third = (int)key.GetValue("Third", Asd.Third ? 1 : 0) == 1 ? true : false;
                Asd.Crosshair = (int)key.GetValue("Crosshair", Asd.Crosshair ? 1 : 0) == 1 ? true : false;
                Asd.HitMarker = (int)key.GetValue("HitMarker", Asd.HitMarker ? 1 : 0) == 1 ? true : false;
                Asd.Imprint = (int)key.GetValue("Imprint", Asd.Imprint ? 1 : 0) == 1 ? true : false;
                Asd.FTD = (int)key.GetValue("FTD", Asd.FTD ? 1 : 0) == 1 ? true : false;

                Asd.ProcessAffinity = new bool[Environment.ProcessorCount];
                for (int i = 0; i < Environment.ProcessorCount; i++)
                {
                    Asd.ProcessAffinity[i] = (int)key.GetValue("ProcessAffinity" + i, 1) == 1 ? true : false;
                }

                /* INT */
                Asd.ServerPort = (int)key.GetValue("ServerPort", Asd.ServerPort);
                Asd.QueryPort = (int)key.GetValue("QueryPort", Asd.QueryPort);
                Asd.RconPort = (int)key.GetValue("RconPort", Asd.RconPort);
                Asd.MaxPlayers = (int)key.GetValue("MaxPlayers", Asd.MaxPlayers);
                Asd.ReservedPlayers = (int)key.GetValue("ReservedPlayers", Asd.ReservedPlayers);
                Asd.ServerX = (int)key.GetValue("ServerX", Asd.ServerX);
                Asd.ServerY = (int)key.GetValue("ServerY", Asd.ServerY);
                Asd.PID = (int)key.GetValue("PID", Asd.PID);

                /* STRING */
                Asd.Pass = (string)key.GetValue("Pass", Asd.Pass);
                Asd.CustomArgs = (string)key.GetValue("CustomArgs", Asd.CustomArgs);
                Asd.ServerPath = (string)key.GetValue("ServerPath", Asd.ServerPath);
                Asd.AltSaveDirectory = (string)key.GetValue("AltSaveDirectory", Asd.AltSaveDirectory);
                Asd.FinalServerPath = string.Empty;
                Asd.ServerIp = (string)key.GetValue("ServerIp", Asd.ServerIp);
                Asd.Loaded = true;
            } else Asd.Loaded = false;
            return Asd;
        }

        private static void SaveRegServers(AtlasServerManager ArkMgr)
        {
            key = Microsoft.Win32.Registry.CurrentUser.OpenSubKey("SOFTWARE\\AtlasServerManager\\Servers", true);
            if (key == null) key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey("SOFTWARE\\AtlasServerManager\\Servers");
            if (key != null)
            {
                int ActIndex = 0, CurActIndex;
                foreach (AtlasServerData Asd in ArkMgr.ServerList.GetServerList()) if (SaveRegServer(ArkMgr, Asd, ActIndex)) ActIndex++;
                foreach (string s in key.GetSubKeyNames()) if ((s.Contains("Server")) && int.TryParse(s.Replace("Server", ""), out CurActIndex) && CurActIndex > ActIndex) key.DeleteSubKey("Server" + CurActIndex);
                key.Close();
            }
        }

        public static bool SaveRegServer(AtlasServerManager ArkMgr, AtlasServerData Asd, int ActIndex, bool DefaultServerSave = false, bool SaveLastOverride = false)
        {
            key = Microsoft.Win32.Registry.CurrentUser.OpenSubKey("SOFTWARE\\AtlasServerManager\\Servers\\" + (DefaultServerSave ? (SaveLastOverride ? "LastSaved" : "Default") : "Server" + ActIndex), true);
            if (key == null) key = Microsoft.Win32.Registry.CurrentUser.CreateSubKey("SOFTWARE\\AtlasServerManager\\Servers\\" + (DefaultServerSave ? (SaveLastOverride ? "LastSaved" : "Default") : "Server" + ActIndex));
            else if (DefaultServerSave && !SaveLastOverride) return true;
            if (key != null)
            {
                /* BOOL */
                key.SetValue("Rcon", Asd.Rcon ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("WildWipe", Asd.WildWipe ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("PVP", Asd.PVP ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("MapB", Asd.MapB ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("Gamma", Asd.Gamma ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("Third", Asd.Third ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("Crosshair", Asd.Crosshair ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("HitMarker", Asd.HitMarker ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("Imprint", Asd.Imprint ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("FTD", Asd.FTD ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);

                if (Asd.ProcessAffinity == null)
                {
                    Asd.ProcessAffinity = new bool[Environment.ProcessorCount];
                    for (int i = 0; i < Environment.ProcessorCount; i++) Asd.ProcessAffinity[i] = true;
                }

                for (int i = 0; i < Environment.ProcessorCount; i++)
                {
                    key.SetValue("ProcessAffinity" + i, Asd.ProcessAffinity[i] ? 1 : 0, Microsoft.Win32.RegistryValueKind.DWord);
                }

                /* INT */
                key.SetValue("ServerPort", Asd.ServerPort, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("QueryPort", Asd.QueryPort, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("RconPort", Asd.RconPort, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("MaxPlayers", Asd.MaxPlayers, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("ReservedPlayers", Asd.ReservedPlayers, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("ServerX", Asd.ServerX, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("ServerY", Asd.ServerY, Microsoft.Win32.RegistryValueKind.DWord);
                key.SetValue("PID", Asd.PID, Microsoft.Win32.RegistryValueKind.DWord);

                /* STRING */
                key.SetValue("Pass", Asd.Pass, Microsoft.Win32.RegistryValueKind.String);
                key.SetValue("CustomArgs", Asd.CustomArgs, Microsoft.Win32.RegistryValueKind.String);
                key.SetValue("ServerPath", Asd.ServerPath, Microsoft.Win32.RegistryValueKind.String);
                key.SetValue("AltSaveDirectory", Asd.AltSaveDirectory, Microsoft.Win32.RegistryValueKind.String);
                key.SetValue("ServerIp", Asd.ServerIp, Microsoft.Win32.RegistryValueKind.String);
                return true;
            }
            return false;
        }
    }
}