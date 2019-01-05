using System.Threading;

namespace AtlasServerManager.Includes
{
    class ArkServerMonitor
    {
        public static void CheckServerStatus(object Data)
        {
            AtlasServerManager ArkMgr = (AtlasServerManager)Data;
            int ServerStatus = 0;
            while (true)
            {
                ArkMgr.Invoke((System.Windows.Forms.MethodInvoker)delegate ()
                {
                    if (!ArkMgr.Updating && ArkMgr.checkBootWhenOff.Checked)
                    {
                        foreach (ArkServerListViewItem ASLVI in ArkMgr.ServerList.Items)
                        {
                            if (ASLVI.Checked && (ServerStatus = ASLVI.GetServerData().IsRunning(ArkMgr)) != 1)
                            {
                                switch(ServerStatus)
                                {
                                    case 0:
                                        ArkMgr.Log("Server: " + ASLVI.Text + " Was Offline Booting Now, Process was not running!");
                                        break;
                                    case 2:
                                        ArkMgr.Log("Server: " + ASLVI.Text + " Was Offline Booting Now, Query Port was not running!");
                                        break;
                                    case 3:
                                        ArkMgr.Log("Server: " + ASLVI.Text + " Booting Now!");
                                        break;
                                    case 4:
                                        ArkMgr.Log("Server: " + ASLVI.Text + " Game Port is offline Attempting Rcon Save Please wait 1 minute!");
                                        continue;
                                }
                                ASLVI.GetServerData().StartServer();
                            }
                            ASLVI.UpdateStatus();
                        }
                    }
                });
                Thread.Sleep((int)(ArkMgr.numServerMonitor.Value) * 1000);
            }
        }
    }
}