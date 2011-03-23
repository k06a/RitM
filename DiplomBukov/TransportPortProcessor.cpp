#include "TransportPortProcessor.h"
#include "network/tcp_header.h"

using namespace DiplomBukov;

TransportPortProcessor::TransportPortProcessor(IProcessorPtr Connector)
    : server_port(0)
    , client_port(0)
{
    setNextProcessor(Connector);
}

IProcessorPtr TransportPortProcessor::CreateCopy() const
{
    IProcessorPtr ptr(new TransportPortProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus TransportPortProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);

    // First session packet (ClientToServer)
    if ((server_port == 0) && (client_port == 0))
    {
        client_port = tcp->src_port;
        server_port = tcp->dst_port;

        char text[10];
        sprintf_s(text, sizeof(text), "TCP_%d", server_port);
        inProt = Protocol(text, server_port);
    }
    
    // Determine direction
    if ((packet->direction() == IPacket::Unknown) && (tcp->src_port != tcp->dst_port))
    {
        bool cts = (tcp->src_port == client_port);
        packet->setDirection(cts ? IPacket::ClientToServer : IPacket::ServerToClient);
    }
    
    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(inProt, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TransportPortProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    
    tcp->src_port = client_port;
    tcp->dst_port = server_port;
    if (packet->direction() == IPacket::ServerToClient)
        std::swap(tcp->src_port, tcp->dst_port);
    
    if (packet->processorBefore(Self) != NULL)
        packet->processorBefore(Self)->backwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

const char * TransportPortProcessor::getProcessorName()
{
    return "TransportPortProcessor";
}

Protocol TransportPortProcessor::getProtocol()
{
    return Protocol::TCP;
}
