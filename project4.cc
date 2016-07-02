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
#include <vector>
// Default Network Topology
//                                  |
//                        Rank 1    |   Rank 0
// ---------------------------------|----------------------------
//   
//           wifi1 10.1.3.0         AP1
//     *    *    *    *    *    *    *
//     |    |    |    |    |    |    |    10.1.1.0    
//    n8    n9  n10  n11  n12  n13   n1------------------ n0   n2   n3   n4   n5   n6    n7
//                                       point-to-point    |    |    |    |    |    |     |
//                                                         *    *    *    *    *    *     *
//                                                         AP0         wifi0 10.1.2.0

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

int 
main (int argc, char *argv[])
{
    bool verbose = true;
    uint32_t n0_Wifi = 6;
    uint32_t n1_Wifi = 6;
    bool pcap = false;
    bool ascii = false;
    
    CommandLine cmd;
    cmd.AddValue ("n0_Wifi", "Number of wifi STA devices", n0_Wifi);
    cmd.AddValue ("n1_Wifi", "Number of wifi STA devices", n1_Wifi);
    cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
    cmd.AddValue ("pcap", "Enable pcap tracing",pcap);
    cmd.AddValue ("ascii", "Enable pcap tracing",ascii);
    cmd.Parse (argc,argv);

    if (n0_Wifi > 250 || n1_Wifi > 250)
    {
        std::cout << "Too many wifi or csma nodes, no more than 250 each" << std::endl;
        exit (1);
    }

    if (verbose)
    {
        LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }
    //创建两个 P2P 节点
    NodeContainer p2pNodes;
    p2pNodes.Create (2);
    //创建 P2P 信道，并设置其参数（数据速率、延时等）
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install (p2pNodes); //将设备安装在 P2P 节点上
    
    //wifi0 的站点及 AP 设置
    NodeContainer wifiStaNodes_0;  //一般站点
    wifiStaNodes_0.Create (n0_Wifi);
    NodeContainer wifiApNode_0 = p2pNodes.Get(0);  //AP0

    //创建无线设备与无线节点之间的互联通道，并将通道对象与物理层对象关联
    //确保所有物理层对象使用相同的底层信道，即无线信道
    YansWifiChannelHelper channel_0 ;//= YansWifiChannelHelper::Default ();
    channel_0.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
    channel_0.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
    YansWifiPhyHelper phy_0 = YansWifiPhyHelper::Default ();
    phy_0.SetChannel (channel_0.Create ());
    
    //配置速率控制算法，AARF 算法
    WifiHelper wifi_0 = WifiHelper::Default ();
    wifi_0.SetRemoteStationManager ("ns3::AarfWifiManager");

    NqosWifiMacHelper mac_0 = NqosWifiMacHelper::Default ();
    
    //配置 mac 类型为 sta 模式，不发送探测请求
    Ssid ssid_0 = Ssid ("ns-3-ssid-0");
    mac_0.SetType ("ns3::StaWifiMac",   //sta 模式
             "Ssid", SsidValue (ssid_0),
             "ActiveProbing", BooleanValue (false));
             
    //创建无线站点设备，将 mac 层和 phy 层安装到设备上
    NetDeviceContainer staDevices_0;
    staDevices_0 = wifi_0.Install (phy_0, mac_0, wifiStaNodes_0);
    
    //配置 AP 节点的 mac 层为 AP 模式
    mac_0.SetType ("ns3::ApWifiMac",   //AP 模式
             "Ssid", SsidValue (ssid_0));  
    //创建 AP 设备
    NetDeviceContainer apDevices_0;
    apDevices_0 = wifi_0.Install (phy_0, mac_0, wifiApNode_0);
    
    /*****************************************************************************/
    //wifi1 的站点及 AP 设置
    NodeContainer wifiStaNodes_1;  //一般站点
    wifiStaNodes_1.Create (n1_Wifi);
    NodeContainer wifiApNode_1 = p2pNodes.Get(1);  //AP1

    //创建无线设备与无线节点之间的互联通道，并将通道对象与物理层对象关联
    //确保所有物理层对象使用相同的底层信道，即无线信道
    YansWifiChannelHelper channel_1 ;//= YansWifiChannelHelper::Default ();
    channel_1.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
    channel_1.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
    YansWifiPhyHelper phy_1 = YansWifiPhyHelper::Default ();
    phy_1.SetChannel (channel_1.Create ());
    
    //配置速率控制算法，AARF 算法
    WifiHelper wifi_1 = WifiHelper::Default ();
    wifi_1.SetRemoteStationManager ("ns3::AarfWifiManager");

    NqosWifiMacHelper mac_1 = NqosWifiMacHelper::Default ();
    
    //配置 mac 类型为 sta 模式，不发送探测请求
    Ssid ssid_1 = Ssid ("ns-3-ssid-1");
    mac_1.SetType ("ns3::StaWifiMac",   //sta 模式
             "Ssid", SsidValue (ssid_1),
             "ActiveProbing", BooleanValue (false));
             
    //创建无线站点设备，将 mac 层和 phy 层安装到设备上
    NetDeviceContainer staDevices_1;
    staDevices_1= wifi_1.Install (phy_1, mac_1, wifiStaNodes_1);
    
    //配置 AP 节点的 mac 层为 AP 模式
    mac_1.SetType ("ns3::ApWifiMac",   //AP 模式
             "Ssid", SsidValue (ssid_1));  
    //创建 AP 设备
    NetDeviceContainer apDevices_1;
    apDevices_1 = wifi_1.Install (phy_1, mac_1, wifiApNode_1);
    
    
    /*****************************************************************************/
    //设置 wifi0 站点及 AP 移动模型
    MobilityHelper mobility0;
    //配置 STA 移动方式，ConstantVelocityMobilityModel，匀速远离 AP 模型
    /*mobility0.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                "Bounds", RectangleValue (Rectangle (-250, 250, -250, 250)));
    mobility0.Install (wifiStaNodes_0);*/
    for (int i = 0;i < (int)n0_Wifi; i ++){
        if(i == 0 || i == 1){
            mobility0.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                    "Bounds", RectangleValue (Rectangle (-250 + 10 *(2 * i - 1), 250+ 10 *(2 * i - 1), 
                                                                         -250+ 10 *(2 * i - 1), 250+ 10 *(2 * i - 1))));
            mobility0.Install ( wifiStaNodes_0.Get(i) );
            Ptr<RandomWalk2dMobilityModel> mob = wifiStaNodes_0.Get(i)->GetObject<RandomWalk2dMobilityModel>();
                mob->SetPosition(Vector(10.0 *(2 * i - 1), 0, 0));
        }
        if(i == 2 || i == 4){
            mobility0.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                    "Bounds", RectangleValue (Rectangle (-250 + 5, 250+ 5, 
                                                                         -250+8.67 *(i - 3), 250+ 8.67 *(i - 3))));
            mobility0.Install ( wifiStaNodes_0.Get(i) );
            Ptr<RandomWalk2dMobilityModel> mob = wifiStaNodes_0.Get(i)->GetObject<RandomWalk2dMobilityModel>();
                mob->SetPosition(Vector(5, 8.67 *(i - 3), 0));
        }
        if(i == 3 || i == 5){
            mobility0.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                    "Bounds", RectangleValue (Rectangle (-250 - 5, 250 + 5, 
                                                                         -250+ 8.67 *(i - 4), 250+ 8.67 *(i - 4))));
            mobility0.Install ( wifiStaNodes_0.Get(i) );
            Ptr<RandomWalk2dMobilityModel> mob = wifiStaNodes_0.Get(i)->GetObject<RandomWalk2dMobilityModel>();
                mob->SetPosition(Vector(-5, 8.67 *(i - 4), 0));
        }
    }
    //配置 AP 移动方式，ConstantPositionMobilityModel，固定位置模型
    mobility0.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility0.Install (wifiApNode_0);
    Ptr<ConstantPositionMobilityModel> mob0 = wifiApNode_0.Get(0)->GetObject<ConstantPositionMobilityModel>();
    mob0->SetPosition(Vector(0.0, 0.0, 0.0));
    
    /*****************************************************************************/
    //设置 wifi1 站点及 AP 移动模型
    MobilityHelper mobility1;
    for (int i = 0; i < (int)n1_Wifi; i ++){
    if(i == 0 || i == 1){
            mobility1.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                    "Bounds", RectangleValue (Rectangle (-250 + 10 *(2 * i - 1), 250+ 10 *(2 * i - 1), 
                                                                         -250+ 10 *(2 * i - 1), 250+ 10 *(2 * i - 1))));
            mobility1.Install ( wifiStaNodes_1.Get(i) );
            Ptr<RandomWalk2dMobilityModel> mob = wifiStaNodes_1.Get(i)->GetObject<RandomWalk2dMobilityModel>();
                mob->SetPosition(Vector(50 + 10.0 *(2 * i - 1), 0, 0));
        }
        if(i == 2 || i == 4){
            mobility1.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                    "Bounds", RectangleValue (Rectangle (-250 + 55, 250+ 55, 
                                                                         -250+8.67 *(i - 3), 250+ 8.67 *(i - 3))));
            mobility1.Install ( wifiStaNodes_1.Get(i) );
            Ptr<RandomWalk2dMobilityModel> mob = wifiStaNodes_1.Get(i)->GetObject<RandomWalk2dMobilityModel>();
                mob->SetPosition(Vector(55, 8.67 *(i - 3), 0));
        }
        if(i == 3 || i == 5){
            mobility1.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                    "Bounds", RectangleValue (Rectangle (-250 + 45, 250 + 45, 
                                                                         -250+ 8.67 *(i - 4), 250+ 8.67 *(i - 4))));
            mobility1.Install ( wifiStaNodes_1.Get(i) );
            Ptr<RandomWalk2dMobilityModel> mob = wifiStaNodes_1.Get(i)->GetObject<RandomWalk2dMobilityModel>();
                mob->SetPosition(Vector(45, 8.67 *(i - 4), 0));
        }
    }
    //配置 AP 移动方式，ConstantPositionMobilityModel，固定位置模型
    mobility1.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility1.Install (wifiApNode_1);    
    Ptr<ConstantPositionMobilityModel> mob1 = wifiApNode_1.Get(0)->GetObject<ConstantPositionMobilityModel>();
    mob1->SetPosition(Vector(50.0, 0.0, 0.0));
    
    //创建节点、设备，
    //信道设置，
    //sta 及 AP 的移动模型，
    //还需配置协议栈
    InternetStackHelper stack;
    stack.Install (wifiApNode_0);
    stack.Install (wifiStaNodes_0);
    stack.Install (wifiApNode_1);
    stack.Install (wifiStaNodes_1);
    
    //地址分配
    Ipv4AddressHelper address;
    //P2P 信道地址分配
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign (p2pDevices);
    //wifi0 信道地址分配
    address.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer wifiInterfaces_0;
    wifiInterfaces_0 = address.Assign (staDevices_0);
    address.Assign (apDevices_0);
    //wifi1 信道地址分配
    address.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer wifiInterfaces_1;
    wifiInterfaces_1 = address.Assign (staDevices_1);
    address.Assign (apDevices_1);


    /*****************************************************************************/
    //设置置 Echo 服务器，端口 9
    UdpEchoServerHelper echoServer (9);

    ApplicationContainer serverApps = 
        echoServer.Install (wifiStaNodes_0.Get (n0_Wifi - 1));  //注意节点是从 0 开始编号的
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (30.0));
    
    //设置 Echo 客户端，并将其指向上面的服务器 iP 地址和端口号
    UdpEchoClientHelper echoClient (wifiInterfaces_0.GetAddress (n0_Wifi - 1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (512));

    //安装其他节点的应用程序，在 wifi1 的站点上安装应用程序
    for (int i = 0; i < (int)n0_Wifi;i ++){
        ApplicationContainer clientApp = 
            echoClient.Install (wifiStaNodes_1.Get (n1_Wifi - 1 - i));
            clientApp.Start (Seconds(3.0 + i));
            clientApp.Stop (Seconds(10.0 + i));
    }
    //在 wifi0 的站点上安装应用程序
    for (int i = 0; i < (int)n0_Wifi;i ++){
        ApplicationContainer clientApp = 
            echoClient.Install (wifiStaNodes_0.Get (n0_Wifi - 1 - i));
            clientApp.Start (Seconds(13.0 + i));
            clientApp.Stop (Seconds(19.0 + i));
    }

    //启动互联网的路由
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    Simulator::Stop (Seconds (30.0));

    if( pcap ==true ){
        pointToPoint.EnablePcapAll ("project4");
        phy_0.EnablePcap ("project4", apDevices_0.Get (0));
        phy_1.EnablePcap ("project4", apDevices_1.Get (0));
    }
    
    if( ascii ==true ){
        AsciiTraceHelper ascii;
        pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("project4-0.tr"));
        phy_0.EnableAsciiAll (ascii.CreateFileStream ("project4-1.tr"));
        phy_1.EnableAsciiAll (ascii.CreateFileStream ("project4-2.tr"));
    }
    
    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}







