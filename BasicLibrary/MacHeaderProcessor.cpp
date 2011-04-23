#include "MacHeaderProcessor.h"
#include "network/mac_header.h"

using namespace DiplomBukov;

MacHeaderProcessor::MacHeaderProcessor(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

ProcessorPtr MacHeaderProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new MacHeaderProcessor(np));
}

ProcessingStatus MacHeaderProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::Ethernet_II) && (proto != Protocol::None))
        return ProcessingStatus::Rejected;

    mac_header * mac = (mac_header *)(&packet->data()[offset]);
    packet->setSrcMac(mac->src);
    packet->setDstMac(mac->dst);
    packet->setFormat((Protocol::NetworkLayer)(int)mac->proto);

    packet->addProcessor(shared_from_this());
    if (nextProcessor != NULL)
    	nextProcessor->forwardProcess(packet->format(), packet, offset + sizeof(mac_header));

	return ProcessingStatus::Accepted;
}

ProcessingStatus MacHeaderProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    int dataLength = packet->size() - offset;

    if (offset < sizeof(mac_header))
    {
        packet->push_front(sizeof(mac_header) - offset);
        offset = sizeof(mac_header);
    }
    offset -= sizeof(mac_header);

    mac_header * mac = (mac_header *)(&packet->data()[offset]);
    mac->src = packet->srcMac();
    mac->dst = packet->dstMac();
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
