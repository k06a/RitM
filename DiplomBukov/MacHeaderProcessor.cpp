#include "MacHeaderProcessor.h"
#include "mac_header.h"

using namespace DiplomBukov;

MacHeaderProcessor::MacHeaderProcessor(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr MacHeaderProcessor::CreateCopy() const
{
    IProcessorPtr ptr(new MacHeaderProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus MacHeaderProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::Ethernet_II) && (proto != Protocol::None))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);
    mac_header * mac = (mac_header *)(&packet->data()[0] + offset);
    packet->src_mac() = mac->src;
    packet->dst_mac() = mac->dst;
    packet->format() = (Protocol::NetworkLayer)(int)mac->proto;

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
    	nextProcessor->forwardProcess(packet->format(), packet, offset + sizeof(mac_header));

	return ProcessingStatus::Accepted;
}

ProcessingStatus MacHeaderProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    int dataLength = packet->size() - offset;

    if (offset < sizeof(mac_header))
    {
        packet->push_front(sizeof(mac_header) - offset);
        offset = sizeof(mac_header);
    }
    offset -= sizeof(mac_header);

    mac_header * mac = (mac_header *)(&packet->data()[0] + offset);
    mac->src = packet->src_mac();
    mac->dst = packet->dst_mac();
    mac->proto = proto.code;

    //if (packet->direction() == IPacket::ServerToClient)
    //    std::swap(mac->src, mac->dst);
    
    //FIX
    //mac->src = "00:22:15:48:78:8e";
    //mac->dst = "00:19:cb:be:28:d3";

    if (prevProcessor != NULL)
        prevProcessor->backwardProcess(Protocol::Ethernet_II, packet, offset);

    return ProcessingStatus::Accepted;
}

Protocol MacHeaderProcessor::getProtocol()
{
    return Protocol::Ethernet_II;
}

const char * MacHeaderProcessor::getProcessorName()
{
    return "MacHeaderProcessor";
}
