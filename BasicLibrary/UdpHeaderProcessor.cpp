#include "UdpHeaderProcessor.h"

using namespace RitM;

UdpHeaderProcessor::UdpHeaderProcessor(ProcessorPtr Connector)
: inproto(Protocol::None)
{
    setNextProcessor(Connector);
}

ProcessorPtr UdpHeaderProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new UdpHeaderProcessor(np));
}

ProcessingStatus UdpHeaderProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    udp_header * udp = (udp_header *)(&(*packet)[offset]);
    header = *udp;

    offset += sizeof(udp_header);

    // First session packet (ClientToServer)
    if (inproto == Protocol::None)
    {
        char text[10];
        sprintf_s(text, sizeof(text), "UDP_%d", (int)udp->dst_port);
        inproto = Protocol(text, udp->dst_port);
    }

    packet->addProcessor(shared_from_this());
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(inproto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus UdpHeaderProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (sizeof(udp_header) > offset)
    {
        int needBytes = sizeof(udp_header) - offset;
        packet->push_front(needBytes);
        offset = sizeof(udp_header);
    }
    offset -= sizeof(udp_header);

    udp_header * udp = (udp_header *)&(*packet)[offset];
    *udp = header;
    udp->length = packet->size() - offset;
    
    if (packet->direction() == IPacket::ServerToClient)
        std::swap(udp->src_port, udp->dst_port);

    if (packet->processorBefore(shared_from_this()) != NULL)
        packet->processorBefore(shared_from_this())->backwardProcess(Protocol::UDP, packet, offset);

    return ProcessingStatus::Accepted;
}

const char * UdpHeaderProcessor::getProcessorName()
{
    return "UdpHeaderProcessor";
}

Protocol UdpHeaderProcessor::getProtocol()
{
    return Protocol::UDP;
}
