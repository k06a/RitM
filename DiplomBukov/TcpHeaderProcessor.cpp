#include "TcpHeaderProcessor.h"
#include "tcp_header.h"

using namespace DiplomBukov;

TcpHeaderProcessor::TcpHeaderProcessor(IProcessorPtr Connector)
    : server_port(header.dst_port)
    , client_port(header.src_port)
    , serverSequenceNumber(header.ack)
    , clientSequenceNumber(header.seq)
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpHeaderProcessor::CreateCopy() const
{
    return IProcessorPtr(new TcpHeaderProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus TcpHeaderProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(packet->data() + offset);

    // Copy header of first packet ClientToServer
    if ((server_port == 0) && (client_port == 0))
    {
        header = *tcp;
    }
    
    // Determine direction
    if ((packet->direction() == IPacket::Unknown) && (tcp->src_port != tcp->dst_port))
    {
        bool cts = (tcp->src_port == client_port);
        packet->setDirection(cts ? IPacket::ServerToClient : IPacket::ClientToServer);
    }

    switch (packet->direction())
    {
        case IPacket::ClientToServer:
            clientSequenceNumber += packet->size() - offset;
            break;
        case IPacket::ServerToClient:
            serverSequenceNumber += packet->size() - offset;
            break;
    }

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(Protocol::None, packet, offset + tcp->header_size());

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpHeaderProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    int dataLength = packet->size() - offset;

    if (offset < sizeof(header))
    {
        packet->push_front(sizeof(header) - offset);
        offset = sizeof(header);
    }

    tcp_header * tcp = (tcp_header *)(packet->data() + offset - sizeof(header));
    *tcp = header;
    if (packet->direction() == IPacket::ServerToClient)
    {
        std::swap(tcp->seq, tcp->ack);
        std::swap(tcp->src_port, tcp->dst_port);
        serverSequenceNumber += dataLength;
    }
    else
    {
        clientSequenceNumber += dataLength;
    }
    
    
    if (prevProcessor != NULL)
        prevProcessor->forwardProcess(Protocol::None, packet, offset + tcp->header_size());

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
