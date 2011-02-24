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

ProcessingStatus MacProcessor::forwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::Ethernet_II) && (proto != Protocol::None))
        return ProcessingStatus::Rejected;

    mac_header * mac = (mac_header *)(packet.data + offset);
    packet.src_hardware_addr = mac->src;
    packet.dst_hardware_addr = mac->dst;

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
