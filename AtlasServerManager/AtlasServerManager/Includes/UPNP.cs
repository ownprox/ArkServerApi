using System.Collections.Generic;
using System.Net;

namespace AtlasServerManager.Includes
{/*
	public class UPNP
	{
        private static string _LocalIPAddress = string.Empty;
        private static string LocalIPAddress
        {
            get
            {
                if (string.IsNullOrEmpty(_LocalIPAddress))
                {
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    if (host.AddressList.Length > 0)_LocalIPAddress = host.AddressList[0].ToString();
                }
                return _LocalIPAddress;
            }
        }

        private static NATUPNPLib.UPnPNATClass UpnpNat;
        private static NATUPNPLib.IStaticPortMappingCollection UpnpMap;
        private static List<int> MappedPorts;

		public static void Init()
		{
            MappedPorts = new List<int>();
			UpnpNat = new NATUPNPLib.UPnPNATClass();
            UpnpMap = UpnpNat.StaticPortMappingCollection;
		}

		public static bool AddUPNPServer(int ServerIndex, int ServerPort, int QueryPort)
		{
            if (MappedPorts.Contains(ServerPort) || MappedPorts.Contains(ServerPort)) return false;
            foreach (NATUPNPLib.IStaticPortMapping EMaps in UpnpMap) if (EMaps.ExternalPort == ServerPort || EMaps.InternalPort == ServerPort || EMaps.ExternalPort == QueryPort || EMaps.InternalPort == QueryPort) return false;
			UpnpMap.Add(ServerPort, "UDP", ServerPort, LocalIPAddress, true, "Ark Server Port #" + ServerIndex.ToString());
			UpnpMap.Add(QueryPort, "UDP", QueryPort, LocalIPAddress, true, "Ark Query Port #" + ServerIndex.ToString());
            MappedPorts.Add(ServerPort);
            MappedPorts.Add(QueryPort);
            return true;
		}

		public static void RemoveUPNPServer(int ServerPort, int QueryPort)
		{
            UpnpMap.Remove(ServerPort, "UDP");
            UpnpMap.Remove(QueryPort, "UDP");
            MappedPorts.Remove(ServerPort);
            MappedPorts.Remove(QueryPort);
		}

        public static void Destroy()
        {
            foreach (int Port in MappedPorts) UpnpMap.Remove(Port, "UDP");
            MappedPorts.Clear();
        }
	}*/
}