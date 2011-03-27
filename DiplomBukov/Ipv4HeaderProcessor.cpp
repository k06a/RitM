#include "Ipv4HeaderProcessor.h"
#include "network/ipv4_header.h"
#include "network/tcp_header.h"

using namespace DiplomBukov;

Ipv4HeaderProcessor::Ipv4HeaderProcessor(ProcessorPtr Connector)
    : header()
{
    setNextProcessor(Connector);
}

ProcessorPtr Ipv4HeaderProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        nextProcessor->CreateCopy();

    return ProcessorPtr(new Ipv4HeaderProcessor(np));
}

ProcessingStatus Ipv4HeaderProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    ipv4_header * ip = (ipv4_header *)(&packet->data()[offset]);
    header.resize(ip->size());
    std::copy(
        packet->data().begin() + offset,
        packet->data().begin() + offset + ip->size(),
        header.begin());

    offset += ip->size();

    packet->addProcessor(this->shared_from_this());
    if (nextProcessor != NULL)
    {
        Protocol::TransportLayer inprot = (Protocol::TransportLayer)ip->proto;
        nextProcessor->forwardProcess(inprot, packet, offset);
    }

    return ProcessingStatus::Accepted;
}

ProcessingStatus Ipv4HeaderProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (sizeof(ipv4_header) > offset)
    {
        int needBytes = sizeof(ipv4_header) - offset;
        packet->push_front(needBytes);
        offset = sizeof(ipv4_header);
    }
    offset -= sizeof(ipv4_header);

    ipv4_header * ip = (ipv4_header *)(&packet->data()[offset]);
    std::copy(header.begin(), header.end(), &packet->data()[offset]);

    ip->proto = proto.code;
    ip->totalLength = packet->data().size() - offset;
    if (packet->direction() == IPacket::ServerToClient)
        std::swap(ip->src_data, ip->dst_data);

    ip->recountSum();
    if (proto == Protocol::TCP)
    {
        tcp_header * tcp = (tcp_header *)(((char*)ip) + ip->size());
        u8 * data = (u8 *)(((char*)tcp) + tcp->header_size());
        int size = ip->totalLength - ip->size() - tcp->header_size();
        tcp->TcpCheckSum(ip, tcp, data, size);
        /*
        tcp->recountSum(
            ip->src_data.dword,
            ip->dst_data.dword,
            ip->totalLength-ip->size());
            */
    }

    if (packet->processorBefore(this->shared_from_this()) != NULL)
        packet->processorBefore(this->shared_from_this())->backwardProcess(getProtocol(), packet, offset);
    
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
