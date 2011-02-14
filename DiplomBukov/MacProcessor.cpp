#include "MacProcessor.h"

using namespace DiplomBukov;

MacProcessor::MacProcessor(IRouter * router_)
	: router_(router_)
{

}

ProcessingStatus MacProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
	if (router_ != NULL)
		router_->transmitPacket(proto, packet, offset);

	return ProcessingStatus::Accepted;
}

void MacProcessor::setRouter(IRouter * router_)
{
	this->router_ = router_;
}

IRouter * MacProcessor::router()
{
	return router_;
}