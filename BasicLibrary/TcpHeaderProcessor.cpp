#include "TcpHeaderProcessor.h"

using namespace DiplomBukov;

TcpHeaderProcessor::TcpHeaderProcessor(ProcessorPtr Connector)
    : inproto(Protocol::None)
{
    setNextProcessor(Connector);
}

ProcessorPtr TcpHeaderProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new TcpHeaderProcessor(np));
}

ProcessingStatus TcpHeaderProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(&(*packet)[offset]);
    header.resize(tcp->header_size());
    std::copy(
        packet->dataBegin() + offset,
        packet->dataBegin() + offset + tcp->header_size(),
        header.begin());

    offset += tcp->header_size();

    // First session packet (ClientToServer)
    if (inproto == Protocol::None)
    {
        char text[10];
        sprintf_s(text, sizeof(text), "TCP_%d", (int)tcp->dst_port);
        inproto = Protocol(text, tcp->dst_port);
    }

    packet->addProcessor(shared_from_this());
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(inproto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpHeaderProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (header.size() > offset)
    {
        int needBytes = header.size() - offset;
        packet->push_front(needBytes);
        offset = header.size();
    }
    offset -= header.size();

    tcp_header * tcp = (tcp_header *)&(*packet)[offset];
    std::copy(header.begin(), header.end(), &(*packet)[offset]);

    //if (packet->direction() == IPacket::ServerToClient)
    //    std::swap(tcp->src_port, tcp->dst_port);

    if (packet->processorBefore(shared_from_this()) != NULL)
        packet->processorBefore(shared_from_this())->backwardProcess(Protocol::TCP, packet, offset);

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
