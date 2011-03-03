#include "MacHeaderProcessor.h"
#include "mac_header.h"

using namespace DiplomBukov;

MacHeaderProcessor::MacHeaderProcessor(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr MacHeaderProcessor::CreateCopy() const
{
    return IProcessorPtr(new MacHeaderProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus MacHeaderProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::Ethernet_II) && (proto != Protocol::None))
        return ProcessingStatus::Rejected;

    mac_header * mac = (mac_header *)(&packet->data()[0] + offset);
    packet->src_mac() = mac->src;
    packet->dst_mac() = mac->dst;
    packet->format() = (Protocol::NetworkLayer)mac->proto;

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
    	nextProcessor->forwardProcess(packet->format(), packet, offset + sizeof(mac_header));

	return ProcessingStatus::Accepted;
}

ProcessingStatus MacHeaderProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    int dataLength = packet->size() - offset;

    if (offset < sizeof(header))
    {
        packet->push_front(sizeof(mac_header) - offset);
        offset = sizeof(mac_header);
    }

    mac_header * mac = (mac_header *)(&packet->data()[0] + offset - sizeof(mac_header));
    mac->src = packet->src_mac();
    mac->dst = packet->dst_mac();
    mac->proto = packet->format();

    if (packet->direction() == IPacket::ServerToClient)
        std::swap(mac->src, mac->dst);
    
    if (prevProcessor != NULL)
        prevProcessor->backwardProcess(Protocol::None, packet, offset - sizeof(mac_header));

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
