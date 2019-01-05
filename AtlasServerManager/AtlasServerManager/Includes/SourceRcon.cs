using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Collections;
using System.Threading;

namespace AtlasServerManager.Includes
{
    public class SourceRconTools
    {
        public static bool ConnectToRcon(SourceRcon Sr, string IP, int Port, string Pass)
        {
                if (Sr != null && !Sr.Connected)
                {
                    int DotCount = 0;
                    for (int i = 0; i < IP.Length; i++) if (IP[i] == '.') DotCount++;
                    if (DotCount != 3)
                    {
                        IPAddress[] ips = Dns.GetHostAddresses(IP);
                        if (ips.Length > 0) IP = ips[0].ToString();
                    }
                    if (IPAddress.TryParse(IP, out IPAddress iP))
                    {
                        bool Connect = Sr.Connect(new IPEndPoint(iP, Port), Pass);
                        int Counter = 0;
                        while (!Sr.Connected)
                        {
                            Thread.Sleep(1000);
                            if (Counter++ > 3)
                            {
                                AtlasServerManager.GetInstance().Log("[Rcon->ConnectToRcon] Something is wrong with connection");
                                return false;
                            }
                            else if (Sr.Connected) break;
                        }
                    }
                }
            return true;
        }

        public static bool SendCommand(string Command, ArkServerListViewItem ASLVI)
        {
            try
            {
                if (!ConnectToRcon(ASLVI.GetServerData().RconConnection, ASLVI.GetServerData().RCONIP == string.Empty ? ASLVI.GetServerData().ServerIp : ASLVI.GetServerData().RCONIP, ASLVI.GetServerData().RconPort, ASLVI.GetServerData().Pass))
                    return false;
                ASLVI.GetServerData().RconConnection.ServerCommand(Command);
                return true;
            }
            catch (Exception e)
            {
                AtlasServerManager.GetInstance().Log("[Rcon->BroadCastMessage] Connection failed: " + e.Message);
                return false;
            }
        }


        public static bool SendCommand(string Command, AtlasServerData ASD)
        {
            try
            {
                if (!ConnectToRcon(ASD.RconConnection, ASD.RCONIP == string.Empty ? ASD.ServerIp : ASD.RCONIP, ASD.RconPort, ASD.Pass))
                    return false;
                ASD.RconConnection.ServerCommand(Command);
                return true;
            }
            catch (Exception e)
            {
                AtlasServerManager.GetInstance().Log("[Rcon->BroadCastMessage] Connection failed: " + e.Message + ", " + ASD.RCONIP == string.Empty ? ASD.ServerIp : ASD.RCONIP);
                return false;
            }
        }

        public static bool SendCommandToAll(string Command)
        {
            try
            {
                bool FirstRun = true;
               // AtlasServerManager.GetInstance().Invoke((System.Windows.Forms.MethodInvoker)delegate ()
               // {
                    foreach (ArkServerListViewItem ASLVI in AtlasServerManager.GetInstance().ServerList.Items)
                    {
                        if (!FirstRun) Thread.Sleep(4000);
                        if (ASLVI.GetServerData().RconConnection == null) ASLVI.GetServerData().RconConnection = new SourceRcon();
                        if (!ConnectToRcon(ASLVI.GetServerData().RconConnection, ASLVI.GetServerData().RCONIP == string.Empty ? ASLVI.GetServerData().ServerIp : ASLVI.GetServerData().RCONIP, ASLVI.GetServerData().RconPort, ASLVI.GetServerData().Pass)) continue;
                        ASLVI.GetServerData().RconConnection.ServerCommand(Command);
                        FirstRun = false;
                    }
              //  });
                return true;
            }
            catch (Exception e)
            {
                AtlasServerManager.GetInstance().Log("[Rcon->BroadCastMessage] Connection failed: " + e.Message);
                return false;
            }
        }

        public static bool SaveWorld()
        {
            bool FirstRun = true;
            int FailCount = 0;
            //AtlasServerManager.GetInstance().Invoke((System.Windows.Forms.MethodInvoker)delegate ()
           // {
                foreach (ArkServerListViewItem ASLVI in AtlasServerManager.GetInstance().ServerList.Items)
                {
                    if (!FirstRun) Thread.Sleep(4000);
                    if (!ConnectToRcon(ASLVI.GetServerData().RconConnection, ASLVI.GetServerData().RCONIP == string.Empty ? ASLVI.GetServerData().ServerIp : ASLVI.GetServerData().RCONIP, ASLVI.GetServerData().RconPort, ASLVI.GetServerData().Pass))
                    {
                        FailCount++;
                        continue;
                    }
                    ASLVI.GetServerData().RconConnection.ServerCommand("DoExit");
                    FirstRun = false;
                }
          //  });
            return FailCount != AtlasServerManager.GetInstance().ServerList.Items.Count;
        }
    }

    /// <summary>
    /// Summary description for SourceRcon.
    /// </summary>
    public class SourceRcon
    {
        public SourceRcon()
        {
#if DEBUG
            TempPackets = new ArrayList();
#endif
        }

        public void Disconnect()
        {
            try
            {
                S.Close();
                connected = false;
            }
            catch (SocketException e)
            {
                Console.WriteLine(e.Message);
            }
        }

        public bool Connect(IPEndPoint Server, string password)
        {
            try
            {
                S = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                PacketCount = 0;
                connected = false;
                S.Connect(Server);

                RCONPacket SA = new RCONPacket
                {
                    RequestId = 1,
                    String1 = password,
                    ServerDataSent = RCONPacket.SERVERDATA_sent.SERVERDATA_AUTH
                };

                SendRCONPacket(SA);

                // This is the first time we've sent, so we can start listening now!
                StartGetNewPacket();
            }
            catch
            {
                OnServerOutput("Error: " + ConnectionFailedString);
                OnConnectionSuccess(false);
                return false;
            }

            return true;
        }

        public void ServerCommand(string command)
        {
            try
            {
                if (connected && S != null && S.Connected)
                {
                    RCONPacket PacketToSend = new RCONPacket();
                    PacketToSend.RequestId = 2;
                    PacketToSend.ServerDataSent = RCONPacket.SERVERDATA_sent.SERVERDATA_EXECCOMMAND;
                    PacketToSend.String1 = command;
                    SendRCONPacket(PacketToSend);
                }
            }
            catch { }
        }

        void SendRCONPacket(RCONPacket p)
        {
            try
            {
                byte[] Packet = p.OutputAsBytes();
                S.BeginSend(Packet, 0, Packet.Length, SocketFlags.None, new AsyncCallback(SendCallback), this);
            }
            catch
            {
                OnServerOutput("Error: Failed to send packet");
            }
        }

        bool connected;
        public bool Connected
        {
            get { return connected; }
        }

        void SendCallback(IAsyncResult ar)
        {
            try
            {
                S.EndSend(ar);
            }
            catch
            {
                OnServerOutput("Error: EndSend Failed");
            }
        }

        int PacketCount;

        void StartGetNewPacket()
        {
            try
            {
                RecState state = new RecState
                {
                    IsPacketLength = true,
                    Data = new byte[4],
                    PacketCount = PacketCount
                };
                PacketCount++;
#if DEBUG
            TempPackets.Add(state);
#endif
                S.BeginReceive(state.Data, 0, 4, SocketFlags.None, new AsyncCallback(ReceiveCallback), state);
            }
            catch
            {
                OnServerOutput("Error: Failed To BeginRecieve");
            }
        }

#if DEBUG
        public ArrayList TempPackets;
#endif

        void ReceiveCallback(IAsyncResult ar)
        {
            bool recsuccess = false;
            RecState state = null;

            try
            {
                int bytesgotten = S.EndReceive(ar);
                state = (RecState)ar.AsyncState;
                state.BytesSoFar += bytesgotten;
                recsuccess = true;

#if DEBUG
                Console.WriteLine("Receive Callback. Packet: {0} First packet: {1}, Bytes so far: {2}", state.PacketCount, state.IsPacketLength, state.BytesSoFar);
#endif

            }
            catch
            {
                connected = false;
                OnServerOutput("Error: " + ConnectionClosed);
            }

            if (recsuccess)
                ProcessIncomingData(state);
        }

        void ProcessIncomingData(RecState state)
        {
            try
            {
                if (state.IsPacketLength)
                {
                    // First 4 bytes of a new packet.
                    state.PacketLength = BitConverter.ToInt32(state.Data, 0);

                    state.IsPacketLength = false;
                    state.BytesSoFar = 0;
                    state.Data = new byte[state.PacketLength];
                    try
                    {
                        S.BeginReceive(state.Data, 0, state.PacketLength, SocketFlags.None, new AsyncCallback(ReceiveCallback), state);
                    }
                    catch
                    {
                        OnServerOutput("Error: Failed BeginReceive");
                    }
                }
                else
                {
                    // Do something with data...

                    if (state.BytesSoFar < state.PacketLength)
                    {
                        try
                        {
                            // Missing data.
                            S.BeginReceive(state.Data, state.BytesSoFar, state.PacketLength - state.BytesSoFar, SocketFlags.None, new AsyncCallback(ReceiveCallback), state);
                        }
                        catch
                        {
                            OnServerOutput("Error: Failed BeginReceive");
                        }
                    }
                    else
                    {
                        // Process data.
#if DEBUG
                    Console.WriteLine("Complete packet.");
#endif

                        RCONPacket RetPack = new RCONPacket();
                        RetPack.ParseFromBytes(state.Data, this);

                        ProcessResponse(RetPack);

                        // Wait for new packet.
                        StartGetNewPacket();
                    }
                }
            }
            catch { }
        }

        void ProcessResponse(RCONPacket P)
        {
            try
            {
                switch (P.ServerDataReceived)
                {
                    case RCONPacket.SERVERDATA_rec.SERVERDATA_AUTH_RESPONSE:
                        if (P.RequestId != -1)
                        {
                            // Connected.
                            connected = true;
                            OnServerOutput(ConnectionSuccessString);
                            OnConnectionSuccess(true);
                        }
                        else
                        {
                            // Failed!
                            OnServerOutput("Error: " + ConnectionFailedString);
                            OnConnectionSuccess(false);
                        }
                        break;
                    case RCONPacket.SERVERDATA_rec.SERVERDATA_RESPONSE_VALUE:
                        //if (hadjunkpacket)
                        {
                            // Real packet!
                            OnServerOutput("Packet: " + P.String1);
                        }
                        // else
                        {
                            //  hadjunkpacket = true;
                            //   OnServerOutput("Info: " + GotJunkPacket);
                        }
                        break;
                    default:
                        OnServerOutput("Error: " + UnknownResponseType);
                        break;
                }
            }
            catch { }
        }

        // bool hadjunkpacket;

        internal void OnServerOutput(string output)
        {
            if (ServerOutput != null)
            {
                ServerOutput(output);
            }
        }

        internal void OnConnectionSuccess(bool info)
        {
            if (ConnectionSuccess != null)
            {
                ConnectionSuccess(info);
            }
        }

        public event StringOutput ServerOutput;
        public event BoolInfo ConnectionSuccess;

        public static string ConnectionClosed = "Connection closed by remote host";
        public static string ConnectionSuccessString = "Connection Succeeded!";
        public static string ConnectionFailedString = "Connection Failed!";
        public static string UnknownResponseType = "Unknown response";
        public static string GotJunkPacket = "Had junk packet. This is normal.";

        Socket S;
    }
    public delegate void StringOutput(string output);
    public delegate void BoolInfo(bool info);
    internal class RecState
    {
        internal RecState()
        {
            PacketLength = -1;
            BytesSoFar = 0;
            IsPacketLength = false;
        }

        public int PacketCount;
        public int PacketLength;
        public int BytesSoFar;
        public bool IsPacketLength;
        public byte[] Data;
    }
    internal class RCONPacket
    {
        internal RCONPacket()
        {
            RequestId = 0;
            String1 = "blah";
            String2 = String.Empty;
            ServerDataSent = SERVERDATA_sent.None;
            ServerDataReceived = SERVERDATA_rec.None;
        }

        internal byte[] OutputAsBytes()
        {
            byte[] packetsize;
            byte[] reqid;
            byte[] serverdata;
            byte[] bstring1;
            byte[] bstring2;

            UTF8Encoding utf = new UTF8Encoding();

            bstring1 = utf.GetBytes(String1);
            bstring2 = utf.GetBytes(String2);

            serverdata = BitConverter.GetBytes((int)ServerDataSent);
            reqid = BitConverter.GetBytes(RequestId);

            // Compose into one packet.
            byte[] FinalPacket = new byte[4 + 4 + 4 + bstring1.Length + 1 + bstring2.Length + 1];
            packetsize = BitConverter.GetBytes(FinalPacket.Length - 4);

            int BPtr = 0;
            packetsize.CopyTo(FinalPacket, BPtr);
            BPtr += 4;

            reqid.CopyTo(FinalPacket, BPtr);
            BPtr += 4;

            serverdata.CopyTo(FinalPacket, BPtr);
            BPtr += 4;

            bstring1.CopyTo(FinalPacket, BPtr);
            BPtr += bstring1.Length;

            FinalPacket[BPtr] = (byte)0;
            BPtr++;

            bstring2.CopyTo(FinalPacket, BPtr);
            BPtr += bstring2.Length;

            FinalPacket[BPtr] = (byte)0;
            BPtr++;

            return FinalPacket;
        }

        internal void ParseFromBytes(byte[] bytes, SourceRcon parent)
        {
            int BPtr = 0;
            ArrayList stringcache;
            UTF8Encoding utf = new UTF8Encoding();

            // First 4 bytes are ReqId.
            RequestId = BitConverter.ToInt32(bytes, BPtr);
            BPtr += 4;
            // Next 4 are server data.
            ServerDataReceived = (SERVERDATA_rec)BitConverter.ToInt32(bytes, BPtr);
            BPtr += 4;
            // string1 till /0
            stringcache = new ArrayList();
            while (bytes[BPtr] != 0)
            {
                stringcache.Add(bytes[BPtr]);
                BPtr++;
            }
            String1 = utf.GetString((byte[])stringcache.ToArray(typeof(byte)));
            BPtr++;

            // string2 till /0

            stringcache = new ArrayList();
            while (bytes[BPtr] != 0)
            {
                stringcache.Add(bytes[BPtr]);
                BPtr++;
            }
            String2 = utf.GetString((byte[])stringcache.ToArray(typeof(byte)));
            BPtr++;

            // Repeat if there's more data?

            if (BPtr != bytes.Length)
            {
                parent.OnServerOutput("Error: " + "Urk, extra data!");
            }
        }

        public enum SERVERDATA_sent : int
        {
            SERVERDATA_AUTH = 3,
            SERVERDATA_EXECCOMMAND = 2,
            None = 255
        }

        public enum SERVERDATA_rec : int
        {
            SERVERDATA_RESPONSE_VALUE = 0,
            SERVERDATA_AUTH_RESPONSE = 2,
            None = 255
        }

        internal int RequestId;
        internal string String1;
        internal string String2;
        internal RCONPacket.SERVERDATA_sent ServerDataSent;
        internal RCONPacket.SERVERDATA_rec ServerDataReceived;
    }
}