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
            PerformanceCounter cpu = new PerformanceCounter("Processor", "% Processor Time", "_Total"), MemA = new PerformanceCounter("Memory", "Available KBytes"), perfCounter = new PerformanceCounter("Process", "% Processor Time", "ShooterGameServer"), MemB = new PerformanceCounter("Memory", "Available KBytes", "ShooterGameServer");

            double AMem;
            GetPhysicallyInstalledSystemMemory(out TotalMemA);
            TotalMem = (TotalMemA / 1048576);
            while (true)
            {
                bool CountPlayers = CountPlayerTick++ >= 3;
                cpu.NextValue();
                Thread.Sleep(5000);
                AMem = ((TotalMemA - MemA.NextValue()) / 1048576);
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
                                {
                                    ASD.SubItems[5].Text = GetCPUPercentByID(perfCounter, ASD.GetServerData().ServerProcess.Id);
                                    if(CountPlayers) ASD.GetServerData().GetPlayersOnline(form1, ASD);
                                }
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

        private static string GetCPUPercentByID(PerformanceCounter perf, int pid)
        {
            string instance = GetInstanceNameForProcessId(pid);
            if (string.IsNullOrEmpty(instance)) return "0%";
            perf.InstanceName = instance;
            string Cpu = (perf.NextValue() / Environment.ProcessorCount).ToString("#");
            return (Cpu.Length == 0 ? "0%" : Cpu + "%");
        }

        private static string GetRamByID(PerformanceCounter perf, int pid)
        {
            string instance = GetInstanceNameForProcessId(pid);
            if (string.IsNullOrEmpty(instance)) return "0.00 GB";
            perf.InstanceName = instance;
            double AMem;
            AMem = ((TotalMemA - perf.NextValue()) / 1048576);
            return AMem.ToString("#.##") + " GB";
        }


        private static string GetInstanceNameForProcessId(int pid)
        {
            PerformanceCounterCategory cat = new PerformanceCounterCategory("Process");
            string[] instances = cat.GetInstanceNames();
            foreach (string instance in instances)
                if (instance.StartsWith("ShooterGameServer"))
                    using (PerformanceCounter cnt = new PerformanceCounter("Process", "ID Process", instance, true))
                        if ((int)cnt.RawValue == pid) return instance;
            return null;
        }
    }
}