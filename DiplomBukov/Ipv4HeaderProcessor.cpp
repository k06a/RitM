#include "Ipv4HeaderProcessor.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

Ipv4HeaderProcessor::Ipv4HeaderProcessor(IProcessorPtr Connector)
    : header()
    , client_ip(header.src_data)
    , server_ip(header.dst_data)
{
    setNextProcessor(Connector);
}

IProcessorPtr Ipv4HeaderProcessor::CreateCopy() const
{
    return IProcessorPtr(new Ipv4HeaderProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus Ipv4HeaderProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    ipv4_header * ip = (ipv4_header *)(&packet->data()[0] + offset);
    if (ip->version != 4)
        return ProcessingStatus::Rejected;

    // Copy header of first packet ClientToServer
    if ((server_ip == 0) && (client_ip == 0))
        header = *ip;

    // Determine direction
    if ((packet->direction() == IPacket::Unknown) && (ip->src_data != ip->dst_data))
    {
        bool cts = (ip->src_data == client_ip);
        packet->setDirection(cts ? IPacket::ClientToServer : IPacket::ServerToClient);
    }

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
    {
        Protocol::TransportLayer prot = (Protocol::TransportLayer)ip->proto;
        nextProcessor->forwardProcess(prot, packet, offset + ip->size());
    }

    return ProcessingStatus::Accepted;
}

const char * Ipv4HeaderProcessor::getProcessorName()
{
    return "Ipv4HeaderProcessor";
}

Protocol Ipv4HeaderProcessor::getProtocol()
{
    return Protocol::IPv4;
}
