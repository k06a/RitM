#include "MacProcessor.h"
#include "mac_header.h"

using namespace DiplomBukov;

MacProcessor::MacProcessor(IProcessorPtr router)
{
    setNextProcessor(router);
}

IProcessorPtr MacProcessor::CreateCopy() const
{
    return IProcessorPtr(new MacProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus MacProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::Ethernet_II) && (proto != Protocol::None))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    mac_header * mac = (mac_header *)(packet->data() + offset);
    packet->src_mac() = mac->src;
    packet->dst_mac() = mac->dst;

    if (nextProcessor != NULL)
    	nextProcessor->forwardProcess(mac->proto, packet, offset + sizeof(mac_header));

	return ProcessingStatus::Accepted;
}

Protocol MacProcessor::getProtocol()
{
    return Protocol::Ethernet_II;
}

const char * MacProcessor::getProcessorName()
{
    return "MacProcessor";
}
