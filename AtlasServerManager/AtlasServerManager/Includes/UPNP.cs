namespace AtlasServerManager.Includes
{
	public class UPNP
	{
        private static string _LocalIPAddress = string.Empty;
        private static string LocalIPAddress
        {
            get
            {
                if (string.IsNullOrEmpty(_LocalIPAddress))
                {
                    System.Net.IPHostEntry host = System.Net.Dns.GetHostEntry(System.Net.Dns.GetHostName());
                    if (host.AddressList.Length > 0)_LocalIPAddress = host.AddressList[0].ToString();
                }
                return _LocalIPAddress;
            }
        }

        private static NATUPNPLib.UPnPNAT UpnpNat;
        private static NATUPNPLib.IStaticPortMappingCollection UpnpMap;

		public static void Init()
		{
			UpnpNat = new NATUPNPLib.UPnPNAT();
            UpnpMap = UpnpNat.StaticPortMappingCollection;
		}

		public static bool AddUPNPServer(int ServerIndex, int ServerPort, int QueryPort, int X, int Y)
		{
            foreach (NATUPNPLib.IStaticPortMapping EMaps in UpnpMap) if (EMaps.ExternalPort == ServerPort || EMaps.ExternalPort == QueryPort) return false;
			UpnpMap.Add(ServerPort, "UDP", ServerPort, LocalIPAddress, true, "Atlas Server X: " + X.ToString() + ", Y: " + Y.ToString());
			UpnpMap.Add(QueryPort, "UDP", QueryPort, LocalIPAddress, true, "Atlas Query X: " + X.ToString() + ", Y: " + Y.ToString());
            return true;
		}

		public static void RemoveUPNPServer(int ServerPort, int QueryPort)
		{
            UpnpMap.Remove(ServerPort, "UDP");
            UpnpMap.Remove(QueryPort, "UDP");
		}

        public static void Destroy()
        {
            foreach (NATUPNPLib.IStaticPortMapping EMaps in UpnpMap)
                if (EMaps.Description.StartsWith("Atlas ")) UpnpMap.Remove(EMaps.ExternalPort, "UDP");
        }
    }
}