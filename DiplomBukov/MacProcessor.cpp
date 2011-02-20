#include "MacProcessor.h"
#include "mac_header.h"

using namespace DiplomBukov;

MacProcessor::MacProcessor(IRouter * router)
	: module(NULL), router(router)
{
}

IProcessor * MacProcessor::CreateCopy() const
{
    return new MacProcessor(router->CreateCopy());
}

ProcessingStatus MacProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::Ethernet_II) && (proto != Protocol::None))
        return ProcessingStatus::Rejected;

    mac_header * mac = (mac_header *)(packet.data + offset);
    packet.src_hardware_addr = mac->src;
    packet.dst_hardware_addr = mac->dst;

    if (router != NULL)
    	router->processPacket(mac->proto, packet, offset + sizeof(mac_header));

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

void MacProcessor::setRouter(IRouter * router)
{
	this->router = router;
}

IRouter * MacProcessor::getRouter()
{
	return router;
}

void MacProcessor::setModule(IProcessorModule * module)
{
    this->module = module;
}

IProcessorModule * MacProcessor::getModule()
{
    return module;
}