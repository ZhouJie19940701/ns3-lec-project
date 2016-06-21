/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

// Default Network Topology
//默认网络拓扑
// Number of wifi or csma nodes can be increased up to 250
//                          |
//                 Rank 0   |   Rank 1
// -------------------------|----------------------------
//   WLAN0 10.1.3.0
//                      AP
//  *    *    *    *    *
//  |    |    |    |    |      10.1.1.0    
// n6   n7   n8   n9   n1 ------------------ n0   n2   n3   n4   n5
//                           point-to-point   |    |    |    |    |
//                                            =====================
//                                                WLAN1 10.1.2.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");		//定义记录组件

int 
main (int argc, char *argv[])
{
    bool verbose = true;
    uint32_t n_0Csma = 4;			//csma节点数量
    uint32_t n_1Csma = 4;				//wifi节点数量
    bool tracing = false;


    CommandLine cmd;
    cmd.AddValue ("n_0Csma", "Number of \"extra\" CSMA nodes/devices", n_0Csma);
    cmd.AddValue ("n_1Csma", "Number of \"extra\" CSMA nodes/devices", n_1Csma);
    cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
    cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

    cmd.Parse (argc,argv);

    // Check for valid number of csma or wifi nodes
    // 250 should be enough, otherwise IP addresses 
    // soon become an issue		//判断是否超过了250个，超过报错 , 原因？
    if (n_0Csma > 250 || n_1Csma > 250)
    {
        std::cout << "Too many csma nodes, no more than 250 each." << std::endl;
        return 1;
    }

    if (verbose)
    {
        LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);	//启动记录组件
    }


    //创建2个 P2P 节点，p2p 链路两端
    NodeContainer p2pNodes;
    p2pNodes.Create (2);
    //创建 P2P 信道，设置信道参数，将设备安装到节点上
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install (p2pNodes); //将设备安装到 P2P 节点上

    //创建LAN0 中的csma节点，包含一个p2p节点
    NodeContainer csmaNodes_0;
    csmaNodes_0.Add (p2pNodes.Get (0));
    csmaNodes_0.Create (n_0Csma);
    //创建 CSMA 信道，设置信道参数，将设备安装到 CSMA 节点上
    CsmaHelper csma_0;
    csma_0.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
    csma_0.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
    NetDeviceContainer csmaDevices_0;
    csmaDevices_0 = csma_0.Install (csmaNodes_0); //将设备安装到 CSMA 节点上

    //创建LAN1 中的csma节点，包含一个p2p节点
    NodeContainer csmaNodes_1;
    csmaNodes_1.Add (p2pNodes.Get (1));
    csmaNodes_1.Create (n_1Csma);
    //创建 CSMA 信道，设置信道参数，将设备安装到 CSMA 节点上
    CsmaHelper csma_1;
    csma_1.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
    csma_1.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
    NetDeviceContainer csmaDevices_1;
    csmaDevices_1 = csma_1.Install (csmaNodes_1); //将设备安装到 CSMA 节点上

    //已经创建了
    //1、节点（NodeContainer：节点最初都是一样的）；
    //2、设备 （NetDeviceContainer：信道.Install(节点)，返回设备）；
    //3、信道（PointToPointHelper、CsmaHelper）；
    //接下来配置协议栈
    InternetStackHelper stack;
    stack.Install (csmaNodes_0);
    stack.Install (csmaNodes_1);

    //分配IP地址
    Ipv4AddressHelper address;
    //P2P信道
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign (p2pDevices);
    //csma信道 LAN0
    address.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer csma_0_Interfaces;
    csma_0_Interfaces = address.Assign (csmaDevices_0);
    //csma信道 LAN1
    address.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer csma_1_Interfaces;
    csma_1_Interfaces = address.Assign (csmaDevices_1);

    //放置echo服务端程序在最右边的csma节点,端口为9
    UdpEchoServerHelper echoServer (9);

    ApplicationContainer serverApps = echoServer.Install (csmaNodes_1.Get (n_1Csma));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

    //回显客户端放在最后的STA节点，指向CSMA网络的服务器，上面的节点地址，端口为9
    UdpEchoClientHelper echoClient (csma_1_Interfaces.GetAddress (n_1Csma), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    //安装其他节点应用程序，在 LAN0 中的站点上安装应用程序
    for(int i = 0; i < (int)n_0Csma; i++){
        ApplicationContainer clientApps = echoClient.Install (csmaNodes_0.Get (n_0Csma - 1 - i));
        clientApps.Start (Seconds (2.0 + i));
        clientApps.Stop (Seconds (10.0));
    }
    //在 LAN1 中的某一个站点上安装应用程序
    ApplicationContainer clientApps = echoClient.Install (csmaNodes_1.Get (n_1Csma - 2));
    clientApps.Start (Seconds (8.0));
    clientApps.Stop (Seconds (10.0));
    //启动互联网络路由
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    Simulator::Stop (Seconds (10.0));

    if (tracing == true)
    {
        pointToPoint.EnablePcapAll ("project2_2");
        csma_0.EnablePcap ("project2_2", csmaDevices_0.Get (0), true);
        csma_1.EnablePcap ("project2_2", csmaDevices_1.Get (0), true);
    }

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}
