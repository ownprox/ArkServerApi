using System;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace AtlasServerManager.Includes
{
    class ArkServerStatusUpdate
    {
        [DllImport("kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool GetPhysicallyInstalledSystemMemory(out long TotalMemoryInKilobytes);
        private static long TotalMemA;
        private static double TotalMem;
        private static int CountPlayerTick = 10;

        public static void UpdateStatus(object Data)
        {
            AtlasServerManager form1 = (AtlasServerManager)Data;
            PerformanceCounter cpu = new PerformanceCounter("Processor", "% Processor Time", "_Total"), MemA = new PerformanceCounter("Memory", "Available KBytes");
            int Players = 0, TotalPlayers = 0;
            double AMem;
            GetPhysicallyInstalledSystemMemory(out TotalMemA);
            TotalMem = TotalMemA / 1048576;
            while (true)
            {
                bool CountPlayers = CountPlayerTick++ >= 3;
                cpu.NextValue();
                Thread.Sleep(3000);
                AMem = (TotalMemA - MemA.NextValue()) / 1048576;
                if (CountPlayers)
                {
                    form1.Invoke((MethodInvoker)delegate ()
                    {
                        if (form1.ServerList.Items.Count > 0)
                        {
                            if (TotalPlayers > 0)
                            {
                                foreach (ArkServerListViewItem ASD in form1.ServerList.Items)
                                {
                                    int.TryParse(ASD.SubItems[5].Text, out Players);
                                    TotalPlayers += Players;
                                }
                            }

                            try
                            {
                                foreach (ArkServerListViewItem ASD in form1.ServerList.Items)
                                {
                                    if (ASD.GetServerData().IsRunning())
                                    {
                                        ASD.GetServerData().GetPlayersOnline(form1, ASD);
                                        TotalPlayers = 1;
                                    }
                                }
                            }
                            catch
                            {
                            }
                        }
                    });
                    CountPlayerTick = 0;
                }
                form1.Text = form1.ASMTitle + " | CPU: " + (int)cpu.NextValue() + "%, Mem: " + AMem.ToString("#.##") + " GB / " + TotalMem + " GB - Players Online: " + TotalPlayers;
                TotalPlayers = 0;
            }
        }
    }
}