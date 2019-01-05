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

        public static void UpdateStatus(object Data)
        {
            AtlasServerManager form1 = (AtlasServerManager)Data;
            PerformanceCounter cpu = new PerformanceCounter("Processor", "% Processor Time", "_Total"), MemA = new PerformanceCounter("Memory", "Available KBytes");
            int Players = 0, TotalPlayers = 0;
            double AMem = 0;
            GetPhysicallyInstalledSystemMemory(out TotalMemA);
            TotalMem = TotalMemA / 1048576;
            while (true)
            {
                try
                {
                    cpu.NextValue();
                    form1.Invoke((MethodInvoker)delegate ()
                    {
                        if (form1.ServerList.Items.Count > 0)
                        {
                            foreach (ArkServerListViewItem ASD in form1.ServerList.Items)
                                if (ASD.GetServerData().IsRunning())
                                    ASD.GetServerData().GetPlayersOnline(form1, ASD);
                        }
                    });

                    Thread.Sleep(3000);
                    AMem = (TotalMemA - MemA.NextValue()) / 1048576;
                    form1.Invoke((MethodInvoker)delegate ()
                    {
                        if (form1.ServerList.Items.Count > 0)
                            foreach (ArkServerListViewItem ASD in form1.ServerList.Items)
                            {
                                int.TryParse(ASD.SubItems[5].Text, out Players);
                                TotalPlayers += Players;
                            }
                    });
                    form1.Text = form1.ASMTitle + " | CPU: " + (int)cpu.NextValue() + "%, Mem: " + AMem.ToString("#.##") + " GB / " + TotalMem + " GB - Players Online: " + TotalPlayers;
                    TotalPlayers = 0;
                    Thread.Sleep(2000);
                }
                catch
                {
                }
            }
        }
    }
}