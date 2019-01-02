using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Threading;
using RconTool.Includes;

namespace RconTool
{
    class Program
    {
        public struct RconServers
        {
            public SourceRcon RconConnection;
            public string IP, Password;
            public int Port;
            public RconServers(string IP, int Port, string Password)
            {
                this.IP = IP;
                this.Port = Port;
                this.Password = Password;
                RconConnection = new SourceRcon();
            }
        };

        private static List<RconServers> Servers = new List<RconServers>();

        static void Main(string[] args)
        {
            Console.Title = "Rcon Tool 1.0";
            if(File.Exists("RconServers.txt"))
            {
                string[] splts;
                using (StreamReader r = new StreamReader("RconServers.txt"))
                {
                    string line = "";
                    while ((line = r.ReadLine()) != null)
                    {
                        splts = line.Split(',');
                        if(splts.Length == 3)
                        {
                            int.TryParse(splts[1], out int Port);
                            Servers.Add(new RconServers(splts[0], Port, splts[2]));
                        }
                    }
                }
            }
            else
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("RconServers.txt not found\n");
                Console.Read();
                return;
            }
            int Type = 0;
            string Data;
            while(true)
            {
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine("Rcon Options:");
                Console.WriteLine("0. Load Plugin");
                Console.WriteLine("1. Remove Plugin");
                Console.WriteLine("2. Broadcast");
                Console.WriteLine("3. SaveWorld");
                Console.WriteLine("4. Custom Rcon Command");
                Console.WriteLine("5. Close Server\n");
                Console.Write("Enter a option: ");
                if (int.TryParse(Console.ReadLine(), out Type))
                {
                    switch(Type)
                    {
                        case 0:
                            Console.Write("\nPlugin Name: ");
                            Data = Console.ReadLine();
                            Console.Clear();
                            Console.ForegroundColor = ConsoleColor.Cyan;
                            Console.WriteLine(SendCommand("plugins.load " + Data) + " Server(s) Loaded Plugin: " + Data + "\n");
                            break;
                        case 1:
                            Console.Write("\nPlugin Name: ");
                            Data = Console.ReadLine();
                            Console.Clear();
                            Console.ForegroundColor = ConsoleColor.Cyan;
                            Console.WriteLine(SendCommand("plugins.unload " + Data) + " Server(s) Removed Plugin: " + Data + "\n");
                            SendCommand("plugins.unload " + Data);
                            break;
                        case 2:
                            Console.Write("\nBroadcase Messgage: ");
                            Data = Console.ReadLine();
                            Console.Clear();
                            Console.ForegroundColor = ConsoleColor.Cyan;
                            Console.WriteLine(SendCommand("broadcast " + Data) + " Server(s) Broadcasted: " + Data + "\n");
                            break;
                        case 3:
                            Console.Clear();
                            Console.ForegroundColor = ConsoleColor.Cyan;
                            Console.WriteLine(SendCommand("saveworld") + " Server(s) Saved \n");
                            break;
                        case 4:
                            Console.Write("\nCustom Command: ");
                            Data = Console.ReadLine();
                            Console.Clear();
                            Console.ForegroundColor = ConsoleColor.Cyan;
                            Console.WriteLine(SendCommand(Data) + " Server(s) Sent Custom Command: " + Data + "\n");
                            break;
                        case 5:
                            Console.Write("\nAre you sure you want to save & close the server (yes or no)?: ");
                            Data = Console.ReadLine();                            
                            Console.Clear();
                            if (Data.ToLower().Equals("yes"))
                            {
                                Console.ForegroundColor = ConsoleColor.Cyan;
                                Console.WriteLine(SendCommand("DoExit") + " Server(s) Shutdown\n");
                            }
                            break;
                    }
                }
                else
                {
                    Console.Clear();
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine("Please Select a option inbetween 0-5\n");
                }
            }
        }

        static int SendCommand(string Command)
        {
            int Count = 0;
            foreach (RconServers Srv in Servers)
            {
                if (!ConnectToRcon(Srv.RconConnection, Srv.IP, Srv.Port, Srv.Password))
                {
                    Console.Clear();
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine("[Rcon->ConnectToRcon] " + Srv.IP + ":" + Srv.Port + " Something is wrong with connection\n");
                    continue;
                }
                Srv.RconConnection.ServerCommand(Command);
                Count++;
            }
            return Count;
        }

        static bool ConnectToRcon(SourceRcon Sr, string IP, int Port, string Pass)
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

                bool Connect = Sr.Connect(new IPEndPoint(IPAddress.Parse(IP), Port), Pass);
                int Counter = 0;
                while (!Sr.Connected)
                {
                    Thread.Sleep(1000);
                    if (Counter++ > 3) return false;
                    else if (Sr.Connected) break;
                }
            }
            return true;
        }
    }
}