#include "MacProcessor.h"
#include "mac_header.h"

using namespace DiplomBukov;

MacProcessor::MacProcessor(IRouter * router_)
	: router_(router_)
{

}

IProcessor * MacProcessor::CreateCopy() const
{
    return new MacProcessor(router_->CreateCopy());
}

ProcessingStatus MacProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::Ethernet_II) && (proto != Protocol::None))
        return ProcessingStatus::Rejected;

    mac_header * mac = (mac_header *)(packet.data + offset);
    if (router_ != NULL)
    	router_->transmitPacket(mac->proto, packet, offset + sizeof(mac_header));

	return ProcessingStatus::Accepted;
}

Protocol MacProcessor::getProtocol()
{
    return Protocol::Ethernet_II;
}

void MacProcessor::setRouter(IRouter * router_)
{
	this->router_ = router_;
}

IRouter * MacProcessor::router()
{
	return router_;
}