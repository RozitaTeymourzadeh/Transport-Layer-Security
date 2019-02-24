#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Point to point conection");

int 
main (int argc, char *argv[])
{

 // NS_LOG_INFO ("Command Line Argument!");
 // CommandLine cmd;
 // cmd.AddValue ("useIpv6", "Use Ipv6", useV6);
 // cmd.Parse (argc, argv);

  NS_LOG_INFO ("Decleare variable!");
  //bool useV6 = false;
  uint16_t router1Port = 9;
  uint16_t router2Port = 10;
  uint16_t receiverPort = 11;
  
 // uint32_t responseSize = 1100;
  uint32_t packetSize = 1100;
  uint32_t maxPacketCount = 2;
  Time interPacketInterval = Seconds (0);

  NS_LOG_INFO ("Report Timing!");
  Time::SetResolution (Time::NS);
  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NS_LOG_INFO ("Create Node!");
  NodeContainer nodes;
  nodes.Create(4);

  InternetStackHelper stack;
  stack.Install(nodes);

  NS_LOG_INFO ("Create link between nodes and set attribute!");
  
  /// PointToPointHelper pointToPoint;
  /// pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  /// pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

  /* Link btw Sender Router1 */
  PointToPointHelper P2PSenderRouter1;
  P2PSenderRouter1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  P2PSenderRouter1.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  P2PSenderRouter1.SetDeviceAttribute ("Mtu", UintegerValue (1100));
  
  /* Link btw Router1 Router2 */
  PointToPointHelper P2PRouter1Router2;
  P2PRouter1Router2.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  P2PRouter1Router2.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  P2PRouter1Router2.SetDeviceAttribute ("Mtu", UintegerValue (1100));

  /* Link btw Router2 Receiver */
  PointToPointHelper P2PRouter2Receiver;
  P2PRouter2Receiver.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  P2PRouter2Receiver.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  P2PRouter2Receiver.SetDeviceAttribute ("Mtu", UintegerValue (1100));


  NS_LOG_INFO ("connect nodes using DeviceContainer!");
  ///NetDeviceContainer devices = pointToPoint.Install(nodes);

  /* Connect node Sender & Router1 */
  NetDeviceContainer deviceSenderRouter1; 
  deviceSenderRouter1 = P2PSenderRouter1.Install(nodes.Get(0),nodes.Get(1));

  /* Connect node Router1 & Router2 */
  NetDeviceContainer deviceRouter1Router2; 
  deviceRouter1Router2 = P2PRouter1Router2.Install(nodes.Get(1),nodes.Get(2));

  /* Connect node Router2 & Receiver */  
  NetDeviceContainer deviceRouter2Receiver; 
  deviceRouter2Receiver = P2PRouter2Receiver.Install(nodes.Get(2),nodes.Get(3));

  NS_LOG_INFO ("Assign IP Addresses!");
  
  Ipv4AddressHelper ipv4Address;
  /* Assign IP to SenderRouter1 */  
  ipv4Address.SetBase ("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaceSenderRouter1;
  interfaceSenderRouter1 = ipv4Address.Assign (deviceSenderRouter1);
  Address SenderRouter1Address;
  SenderRouter1Address = Address(interfaceSenderRouter1.GetAddress (1));

  /* Assign IP to Router1Router2 */  
  ipv4Address.SetBase ("10.0.2.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaceRouter1Router2;
  interfaceRouter1Router2= ipv4Address.Assign (deviceRouter1Router2);
  Address Router1Router2Address;
  Router1Router2Address = Address(interfaceRouter1Router2.GetAddress (1));

  /* Assign IP to Router2Receiver */
  ipv4Address.SetBase ("10.0.3.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaceRouter2Receiver;
  interfaceRouter2Receiver= ipv4Address.Assign (deviceRouter2Receiver);
  Address Router2ReceiverAddress;
  Router2ReceiverAddress = Address(interfaceRouter2Receiver.GetAddress (1));



  ///if (useV6 == false)
  ///{
  ///   Ipv4AddressHelper ipv4Address;
  ///   ipv4Address.SetBase ("10.1.1.0", "255.255.255.0");
  ///   Ipv4InterfaceContainer interfaces = ipv4Address.Assign (devices);
  ///   serverAddress = Address(interfaces.GetAddress (1));
  ///}
  ///else
  ///{
  ///  Ipv6AddressHelper ipv6Address;
  ///    ipv6Address.SetBase ("2001:0000:f00d:cafe::", Ipv6Prefix (64));
  ///    Ipv6InterfaceContainer interfaces = ipv6Address.Assign (devices);
  ///    serverAddress = Address(interfaces.GetAddress (1,1));
  ///  }

  // assign IP to nodes and generate interface from node and IP
  ///Ipv4AddressHelper address;
  ///address.SetBase("10.1.1.0", "255.255.255.0");
  ///Ipv4InterfaceContainer interfaces = address.Assign (devices);

  NS_LOG_INFO ("Create Server Applications!");
  /* Create Server on Router 1 */
  UdpEchoServerHelper echoRouter1Server (router1Port);
  //echoRouter1Server.SetAttribute ("PacketSize", UintegerValue (responseSize));
  ApplicationContainer router1ServerApps = echoRouter1Server.Install (nodes.Get (1));
  router1ServerApps.Start (Seconds (1.0));
  router1ServerApps.Stop (Seconds (10.0));

  /* Create Server on Router 2 */
  UdpEchoServerHelper echoRouter2Server (router2Port);
  //echoRouter2Server.SetAttribute ("PacketSize", UintegerValue (responseSize));
  ApplicationContainer router2ServerApps = echoRouter2Server.Install (nodes.Get (2));
  router2ServerApps.Start (Seconds (1.0));
  router2ServerApps.Stop (Seconds (10.0));

  /* Create Server on Receiver */
  UdpEchoServerHelper echoReceiverServer (receiverPort);
  //echoReceiverServer.SetAttribute ("PacketSize", UintegerValue (responseSize));
  ApplicationContainer receiverServerApps = echoReceiverServer.Install (nodes.Get (3));
  receiverServerApps.Start (Seconds (1.0));
  receiverServerApps.Stop (Seconds (10.0));


  //UdpEchoServerHelper echoServer (port);
  //echoServer.SetAttribute ("PacketSize", UintegerValue (responseSize));
  //ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  //serverApps.Start (Seconds (1.0));
  //serverApps.Stop (Seconds (10.0));



  NS_LOG_INFO ("Create Client Applications!");

  /* Create Client on Sender */
  UdpEchoClientHelper echoSenderClient (SenderRouter1Address, router1Port);
  echoSenderClient.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  echoSenderClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
  echoSenderClient.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer senderClientApps = echoSenderClient.Install(nodes.Get(0));
  senderClientApps.Start (Seconds (2.0));
  senderClientApps.Stop (Seconds (10.0));

  /* Create Client on Router1 */
  UdpEchoClientHelper echoRouter1Client (Router1Router2Address, router2Port);
  echoRouter1Client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  echoRouter1Client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  echoRouter1Client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer router1ClientApps = echoRouter1Client.Install(nodes.Get(1));
  router1ClientApps.Start (Seconds (2.0));
  router1ClientApps.Stop (Seconds (10.0));

  /* Create Client on Router2 */
  UdpEchoClientHelper echoRouter2Client (Router2ReceiverAddress, receiverPort);
  echoRouter2Client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  echoRouter2Client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  echoRouter2Client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer router2ClientApps = echoRouter2Client.Install(nodes.Get(2));
  router2ClientApps.Start (Seconds (2.0));
  router2ClientApps.Stop (Seconds (10.0));




  //UdpEchoClientHelper echoClient (routerAddress, port);
  //echoClient.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  //echoClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
  //echoClient.SetAttribute ("PacketSize", UintegerValue (packetSize));
 
  //ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
  //clientApps.Start (Seconds (2.0));
  //clientApps.Stop (Seconds (10.0));




  // create interface btw client and server, set attribute
  ///UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
  ///echoClient.SetAttribute("MaxPackets", UintegerValue(1));
  ///echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
  ///echoClient.SetAttribute("PacketSize", UintegerValue(1024));

  // create application on top of client
  ///ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
  ///clientApps.Start(Seconds(2.0));
  ///clientApps.Stop(Seconds(10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  P2PSenderRouter1.EnablePcapAll ("SenderRouter1");
  P2PRouter1Router2.EnablePcapAll ("Router1Router2");
  P2PRouter2Receiver.EnablePcapAll ("Router2Receiver");

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}











 

 
