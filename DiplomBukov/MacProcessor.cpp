#include "MacProcessor.h"

using namespace DiplomBukov;

MacProcessor::MacProcessor(IRouter * router_)
	: router_(router_)
{

}

ProcessingStatus MacProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if (proto != Protocol::None)
        return ProcessingStatus::Rejected;

    proto = *(unsigned short*)(packet.data + offset + 12);
    offset += 14;
	if (router_ != NULL)
    	router_->transmitPacket(proto, packet, offset);

	return ProcessingStatus::Accepted;
}

Protocol MacProcessor::getProtocol()
{
    return Protocol::None;
}

void MacProcessor::setRouter(IRouter * router_)
{
	this->router_ = router_;
}

IRouter * MacProcessor::router()
{
	return router_;
}