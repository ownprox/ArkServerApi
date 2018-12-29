using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace AtlasServerManager.Includes
{
    class Worker
    {
        private enum WorkerType
        {
            StatusUdpdate = 0,
            ServerStatusCheck,
            ServerUpdateCheck,
            Max,
        }
        private static Thread[] Workers;
  
        private static void AddWorker(WorkerType Index, ParameterizedThreadStart target, object form, bool Start = true)
        {
            Workers[(int)Index] = new Thread(target);
            if (Start) Workers[(int)Index].Start(form);
        }

        public static void Start(AtlasServerManager form1)
        {
            Workers = new Thread[(int)WorkerType.Max];
            AddWorker(WorkerType.StatusUdpdate, new ParameterizedThreadStart(ArkServerStatusUpdate.UpdateStatus), form1);
            AddWorker(WorkerType.ServerStatusCheck, new ParameterizedThreadStart(ArkServerMonitor.CheckServerStatus), form1);
            //AddWorker(WorkerType.ServerUpdateCheck, new ParameterizedThreadStart(ArkServerUpdater.CheckForUpdates), form1);
        }

        public static void Destroy() { foreach (Thread w in Workers) if (w != null) w.Abort(); }
    }
}