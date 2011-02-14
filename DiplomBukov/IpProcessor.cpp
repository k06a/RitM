#include "IpProcessor.h"

using namespace DiplomBukov;

IpProcessor::IpProcessor(IRouter * router_)
	: router_(router_)
{
}

ProcessingStatus IpProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
	if (router_ != 0)
		router_->transmitPacket(proto, packet, offset);

	return ProcessingStatus::Accepted;
}

void IpProcessor::setRouter(IRouter * router_)
{
	this->router_ = router_;
}

IRouter * IpProcessor::router()
{
	return router_;
}