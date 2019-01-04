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

            double AMem;
            GetPhysicallyInstalledSystemMemory(out TotalMemA);
            TotalMem = TotalMemA / 1048576;
            while (true)
            {
                bool CountPlayers = CountPlayerTick++ >= 3;
                cpu.NextValue();
                Thread.Sleep(5000);
                AMem = (TotalMemA - MemA.NextValue()) / 104857;
                form1.Invoke((MethodInvoker)delegate ()
                {
                    form1.Text = "Atlas Server Manager | CPU: " + (int)cpu.NextValue() + "%, Mem: " + AMem.ToString("#.##") + " GB / " + TotalMem + " GB";
                    if (form1.ServerList.Items.Count > 0)
                    {
                        try
                        {
                            foreach (ArkServerListViewItem ASD in form1.ServerList.Items)
                            {
                                if (ASD.GetServerData().IsRunning())
                                    if(CountPlayers) ASD.GetServerData().GetPlayersOnline(form1, ASD);
                            }
                        }
                        catch
                        {
                        }
                    }
                });
                if (CountPlayers) CountPlayerTick = 0;
            }
        }
    }
}