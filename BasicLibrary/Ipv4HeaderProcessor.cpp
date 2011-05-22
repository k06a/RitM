#include "Ipv4HeaderProcessor.h"
#include "network/ipv4_header.h"
#include "network/tcp_header.h"
#include "network/udp_header.h"

using namespace RitM;

Ipv4HeaderProcessor::Ipv4HeaderProcessor(ProcessorPtr Connector)
    : header()
{
    setNextProcessor(Connector);
}

ProcessorPtr Ipv4HeaderProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new Ipv4HeaderProcessor(np));
}

ProcessingStatus Ipv4HeaderProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    ipv4_header * ip = (ipv4_header *)(&(*packet)[offset]);
    header.resize(ip->size());
    std::copy(
        packet->dataBegin() + offset,
        packet->dataBegin() + offset + ip->size(),
        header.begin());

    offset += ip->size();

    packet->addProcessor(shared_from_this());
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

    ipv4_header * ip = (ipv4_header *)(&(*packet)[offset]);
    std::copy(header.begin(), header.end(), &(*packet)[offset]);

    ip->proto = proto.code;
    ip->totalLength = packet->size() - offset;
    //if (packet->direction() == IPacket::ServerToClient)
    //    std::swap(ip->src_data, ip->dst_data);

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
    } else if (proto == Protocol::UDP)
    {
        udp_header * udp = (udp_header *)(((char*)ip) + ip->size());
        u8 * data = (u8*)(((char*)udp) + udp->header_size());
        int size = ip->totalLength - ip->size() - udp->header_size();
        //udp->checksum = udp->udp_sum_calc(data, size, ip->src_data.dword, ip->dst_data.dword);
        udp->checksum = 0;//udp->inet_chksum_pseudo(data, ip->src_data.dword, ip->dst_data.dword, 17, size);
    }

    if (packet->processorBefore(shared_from_this()) != NULL)
        packet->processorBefore(shared_from_this())->backwardProcess(getProtocol(), packet, offset);
    
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
