#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/applications-module.h"

using namespace ns3;

int main(int argc, char *argv[]) {
    CommandLine cmd;
    cmd.Parse(argc, argv);

    Ptr <LteHelper> lteHelper = CreateObject<LteHelper>();
    lteHelper->SetSchedulerType("ns3::PfFfMacScheduler");

    Ptr <PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>();
    lteHelper->SetEpcHelper(epcHelper);

    Ptr <Node> pgw = epcHelper->GetPgwNode();

    NodeContainer ueNodes;
    NodeContainer enbNodes;
    enbNodes.Create(1);
    ueNodes.Create(2);

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(enbNodes);
    mobility.Install(ueNodes);

    NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice(enbNodes);
    NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice(ueNodes);

    InternetStackHelper internet;
    internet.Install(ueNodes);

    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs));

    lteHelper->Attach(ueLteDevs, enbLteDevs.Get(0));

    ApplicationContainer clientApps;
    ApplicationContainer serverApps;

    for (uint32_t u = 0; u < ueNodes.GetN(); ++u) {
        uint16_t dlPort = 1234 + u;
        uint16_t ulPort = 2000 + u;

        UdpClientHelper dlClient(ueIpIface.GetAddress(u), dlPort);
        dlClient.SetAttribute("MaxPackets", UintegerValue(0xFFFFFFFF));
        dlClient.SetAttribute("Interval", TimeValue(Seconds(0.01)));
        dlClient.SetAttribute("PacketSize", UintegerValue(1024));

        clientApps.Add(dlClient.Install(pgw));

        PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
        serverApps.Add(dlPacketSinkHelper.Install(ueNodes.Get(u)));

        UdpClientHelper ulClient(epcHelper->GetUeDefaultGatewayAddress(), ulPort);
        ulClient.SetAttribute("MaxPackets", UintegerValue(0xFFFFFFFF));
        ulClient.SetAttribute("Interval", TimeValue(Seconds(0.01)));
        ulClient.SetAttribute("PacketSize", UintegerValue(1024));

        clientApps.Add(ulClient.Install(ueNodes.Get(u)));

        PacketSinkHelper ulPacketSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), ulPort));
        serverApps.Add(ulPacketSinkHelper.Install(pgw));
    }

    serverApps.Start(Seconds(0.01));
    clientApps.Start(Seconds(0.01));

    lteHelper->EnableTraces();

    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
