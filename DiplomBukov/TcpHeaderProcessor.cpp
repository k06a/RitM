#include "TcpHeaderProcessor.h"

using namespace DiplomBukov;

TcpHeaderProcessor::TcpHeaderProcessor(IProcessorPtr Connector)
    : inproto(Protocol::None)
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpHeaderProcessor::CreateCopy() const
{
    IProcessorPtr ptr(new TcpHeaderProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus TcpHeaderProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(&packet->data()[offset]);
    offset += tcp->header_size();
    u16 server_port =
        (packet->direction() == IPacket::ClientToServer)
        ? tcp->dst_port : tcp->src_port;

    // First session packet (ClientToServer)
    if (inproto == Protocol::None)
    {
        char text[10];
        sprintf_s(text, sizeof(text), "TCP_%d", server_port);
        inproto = Protocol(text, server_port);
    }

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(inproto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpHeaderProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (sizeof(tcp_header) > offset)
    {
        int needBytes = sizeof(tcp_header) - offset;
        packet->push_front(needBytes);
        offset = sizeof(tcp_header);
    }
    offset -= sizeof(tcp_header);

    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    //memset(tcp, 0, sizeof(tcp_header));
    
    //tcp->recountSum()

    if (packet->prevProcessor(Self) != NULL)
        packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);

    return ProcessingStatus::Accepted;
}

const char * TcpHeaderProcessor::getProcessorName()
{
    return "TcpHeaderProcessor";
}

Protocol TcpHeaderProcessor::getProtocol()
{
    return Protocol::TCP;
}
