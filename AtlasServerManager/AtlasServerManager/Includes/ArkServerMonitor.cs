using System.Threading;

namespace AtlasServerManager.Includes
{
    class ArkServerMonitor
    {
        public static void CheckServerStatus(object Data)
        {
            AtlasServerManager ArkMgr = (AtlasServerManager)Data;
            while (true)
            {
                ArkMgr.Invoke((System.Windows.Forms.MethodInvoker)delegate ()
                {
                    if (!ArkMgr.Updating && ArkMgr.checkBootWhenOff.Checked)
                    {
                        foreach (ArkServerListViewItem ASLVI in ArkMgr.ServerList.Items)
                        {
                            if (ASLVI.Checked && !ASLVI.GetServerData().IsRunning())
                            {
                                ArkMgr.Log("Server Port: " + ASLVI.Text + " Was Offline Booting Now!");
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