#include "UdpHeaderProcessor.h"

using namespace DiplomBukov;

UdpHeaderProcessor::UdpHeaderProcessor(IProcessorPtr Connector)
    : TcpHeaderProcessor(Connector)
    , server_port(header.dst_port)
    , client_port(header.src_port)
{
}

ProcessingStatus UdpHeaderProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    udp_header * udp = (udp_header *)(&packet->data()[0] + offset);

    // Copy header of first packet ClientToServer
    if ((server_port == 0) && (client_port == 0))
        header = *udp;

    // Determine direction
    if ((packet->direction() == IPacket::Unknown) && (udp->src_port != udp->dst_port))
    {
        bool cts = (udp->src_port == client_port);
        packet->setDirection(cts ? IPacket::ClientToServer : IPacket::ServerToClient);
    }

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(Protocol::None, packet, offset + sizeof(header));

    return ProcessingStatus::Accepted;
}

ProcessingStatus UdpHeaderProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    int dataLength = packet->size() - offset;

    if (offset < sizeof(header))
    {
        packet->push_front(sizeof(header) - offset);
        offset = sizeof(header);
    }

    udp_header * udp = (udp_header *)(&packet->data()[0] + offset - sizeof(header));
    *udp = header;
    if (packet->direction() == IPacket::ServerToClient)
        std::swap(udp->src_port, udp->dst_port);
    
    if (prevProcessor != NULL)
        prevProcessor->backwardProcess(Protocol::None, packet, offset - sizeof(header));

    return ProcessingStatus::Accepted;
}

Protocol UdpHeaderProcessor::getProtocol()
{
    return Protocol::UDP;
}

const char * UdpHeaderProcessor::getProcessorName()
{
    return "UdpHeaderProcessor";
}