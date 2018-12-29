using System;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Collections.Generic;

namespace AtlasServerManager.Includes
{
    class SourceQuery : IDisposable
    {
        public struct ServerData
        {
            public byte Protocol, Players, MaxPlayers, Bots, Dedicated, Os;
            public string Name, Map, ModDir, ModDesc;
            public short AppID;
            public bool PasswordProtected, Secure;
            public int Ping;

            public ServerData(byte Protocol, string Name, string Map, string ModDir, string ModDesc, short AppID, byte Players, byte MaxPlayers, byte Bots, byte Dedicated, byte Os, bool PasswordProtected, bool Secure)
            {
                this.Protocol = Protocol;
                this.Name = Name;
                this.Map = Map;
                this.ModDir = ModDir;
                this.ModDesc = ModDesc;
                this.AppID = AppID;
                this.Players = Players;
                this.MaxPlayers = MaxPlayers;
                this.Bots = Bots;
                this.Dedicated = Dedicated;
                this.Os = Os;
                this.PasswordProtected = PasswordProtected;
                this.Secure = Secure;
                Ping = 0;
            }
        };

        public struct Players
        {
            public string Name;
            public long Score;
            public float OnlineTime;
            public Players(string Name, long Score, float OnlineTime)
            {
                this.Name = Name;
                this.Score = Score;
                this.OnlineTime = OnlineTime;
            }
        };

        private long timeGetTime() { return DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond; }
        private List<Players> PlayersOnline = new List<Players>();
        private ServerData SrvData;
        private Action<ServerData> ServerListCallbackRemote;
        private Socket SourceQuerySocket;
        private static int BuffSize = 6144;
        private const byte Send_INFO = 0x54, Recieve_INFO = 0x49, Send_PING = 0x69, Recieve_PING = 0x6A, Send_PLAYER = 0x55, Recieve_PLAYER = 0x44, Send_RULES = 0x56, Recieve_RULES = 0x45, Send_CHALLENGE = 0x57, Recieve_CHALLENGE = 0x41;
        private byte[] HEADER = new byte[] { 0xFF, 0xFF, 0xFF, 0xFF }, BUFFER = new byte[BuffSize];
        private byte ChallengePacket = 0;
        private int ReadIndex = 4;
        private long Ping;

        private byte[] BuildPacket(byte PacketID, byte[] Data, int Size)
        {
            byte[] Packet = new byte[5 + Data.Length];
            Buffer.BlockCopy(HEADER, 0, Packet, 0, 4);
            Packet[4] = PacketID;
            Buffer.BlockCopy(Data, 0, Packet, 5, Size != -1 ? Size : Data.Length);
            return Packet;
        }

        private byte ReadByte(ref byte[] _bSource)
        {
            if (_bSource == null || _bSource.Length == 0) return 0;
            byte b = _bSource[ReadIndex++];
            return b;
        }

        private short ReadShort(ref byte[] _bSource)
        {
            if (_bSource == null || _bSource.Length == 0) return 0;
            short s = 0;
            try
            {
                s = BitConverter.ToInt16(_bSource, ReadIndex);
            }
            catch { }
            ReadIndex += 2;
            return s;
        }

        private int ReadInt(ref byte[] _bSource)
        {

            if (_bSource == null || _bSource.Length == 0) return 0;
            int l = 0;
            try
            {
                l = BitConverter.ToInt32(_bSource, ReadIndex);
            }
            catch { }
            ReadIndex += 4;
            return l;
        }

        private long ReadLong(ref byte[] _bSource)
        {

            if (_bSource == null || _bSource.Length == 0) return 0;
            long l = 0;
            try
            {
                l = BitConverter.ToInt64(_bSource, ReadIndex);
            }
            catch { }
            ReadIndex += 4;
            return l;
        }

        private float ReadFloat(ref byte[] _bSource)
        {

            if (_bSource == null || _bSource.Length == 0) return 0;
            float f = 0;
            try
            {
                f = BitConverter.ToSingle(_bSource, ReadIndex);
            }
            catch { }
            ReadIndex += 4;
            return f;
        }

        private string ReadString(ref byte[] _bSource)
        {
            if (_bSource == null || _bSource.Length == 0) return string.Empty;
            StringBuilder sb = new StringBuilder();
            int i = ReadIndex, iLen = 0;
            while (i < _bSource.Length)
            {
                if (_bSource[i] == 0) i = _bSource.Length;
                else sb.Append(Convert.ToChar(_bSource[i++]));
                iLen++;
            }
            ReadIndex += iLen;
            return sb.ToString();
        }

        private void ConnectCallback(IAsyncResult ar)
        {  
            try
            {
                Socket client = (Socket) ar.AsyncState;
                client.EndConnect(ar);
                SendPacket(client, Send_INFO);
            } catch { }
        }

        private void SendChallenge(byte PacketID)
        {
            ChallengePacket = PacketID;
            Ping = timeGetTime();
            SendPacket(SourceQuerySocket, PacketID, new byte[4] { 0xFF, 0xFF, 0xFF, 0xFF });
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            try
            {
                int bytesRead = SourceQuerySocket.EndReceive(ar);
                if (bytesRead > 0)
                {
                    ReadIndex = 4;
                    switch (ReadByte(ref BUFFER))
                    {
                        case Recieve_INFO:
                            SrvData = new ServerData(ReadByte(ref BUFFER), ReadString(ref BUFFER), ReadString(ref BUFFER), ReadString(ref BUFFER), ReadString(ref BUFFER), ReadShort(ref BUFFER), ReadByte(ref BUFFER), ReadByte(ref BUFFER), ReadByte(ref BUFFER), ReadByte(ref BUFFER), ReadByte(ref BUFFER), (ReadByte(ref BUFFER) == 1), (ReadByte(ref BUFFER) == 1));
                            SendChallenge(Send_PLAYER);
                            break;
                        case Recieve_CHALLENGE:
                            SrvData.Ping = (int)((timeGetTime() - Ping) / 2);
                            switch (ChallengePacket)
                            {
                                case Send_PLAYER:
                                    int Challenge = ReadInt(ref BUFFER);
                                    SendPacket(SourceQuerySocket, Send_PLAYER, BitConverter.GetBytes(Challenge), 4);
                                    break;
                            }
                            break;
                        case Recieve_PLAYER:
                            {
                                string Name;
                                long Score;
                                float OnlineTime;
                                for (int i = 0, Count = ReadByte(ref BUFFER); i < Count; i++)
                                {
                                    ReadByte(ref BUFFER);
                                    Name = ReadString(ref BUFFER);
                                    Score = ReadLong(ref BUFFER);
                                    OnlineTime = ReadFloat(ref BUFFER);
                                    if (Name != string.Empty) PlayersOnline.Add(new Players(Name, Score, OnlineTime));
                                }
                                SrvData.Players = (byte)PlayersOnline.Count;
                                PlayersOnline.Clear();
                                SourceQuerySocket.Shutdown(SocketShutdown.Both);
                                SourceQuerySocket.Close();
                                ServerListCallbackRemote(SrvData);
                            }
                            break;
                    }
                   
                }
            } catch { }
        }

        private void SendPacket(Socket client, byte PacketID, byte[] ExtraData = null, int Size = -1)
        {
            if (ExtraData == null) ExtraData = new byte[] { 83, 111, 117, 114, 99, 101, 32, 69, 110, 103, 105, 110, 101, 32, 81, 117, 101, 114, 121, 0 }; //Source Engine Query\0
            byte[] msg = BuildPacket(PacketID, ExtraData, Size);
            SourceQuerySocket.BeginSend(msg, 0, msg.Length, 0, new AsyncCallback(SendCallback), 0);
            client.BeginReceive(BUFFER, 0, BuffSize, 0, new AsyncCallback(ReceiveCallback), 0);
        }

        private void SendCallback(IAsyncResult ar) 
        {
            try
            {
                SourceQuerySocket.EndSend(ar);
            } catch { }
        }

        public SourceQuery(string IP, int Port, Action<ServerData> CallBack)
        {
            try
            {
                ServerListCallbackRemote = CallBack;
                IPHostEntry ipHostInfo = Dns.Resolve(IP);
                IPAddress ipAddress = ipHostInfo.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, Port);
                SourceQuerySocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                SourceQuerySocket.BeginConnect(remoteEP, new AsyncCallback(ConnectCallback), SourceQuerySocket);
            }
            catch { }
        }
        
        public void Dispose()
        {
            GC.SuppressFinalize(this);
        }

        ~SourceQuery()
        {
            Dispose();
        }
    }
}