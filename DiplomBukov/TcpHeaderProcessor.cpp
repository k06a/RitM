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
    header.resize(tcp->header_size());
    std::copy(
        packet->data().begin() + offset,
        packet->data().begin() + offset + tcp->header_size(),
        header.begin());

    offset += tcp->header_size();

    // First session packet (ClientToServer)
    if (inproto == Protocol::None)
    {
        char text[10];
        sprintf_s(text, sizeof(text), "TCP_%d", (int)tcp->dst_port);
        inproto = Protocol(text, tcp->dst_port);
    }

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(inproto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpHeaderProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (header.size() > offset)
    {
        int needBytes = header.size() - offset;
        packet->push_front(needBytes);
        offset = header.size();
    }
    offset -= header.size();

    tcp_header * tcp = (tcp_header *)&packet->data()[offset];
    std::copy(header.begin(), header.end(), &packet->data()[offset]);
    
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
