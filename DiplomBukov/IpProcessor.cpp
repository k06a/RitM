#include "IpProcessor.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

IpProcessor::IpProcessor(IRouter * router_)
	: router_(router_)
{
}

IProcessor * IpProcessor::CreateCopy() const
{
    return new IpProcessor(router_->CreateCopy());
}

ProcessingStatus IpProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != Protocol::IP))
        return ProcessingStatus::Rejected;

    ipv4_header * ipv4 = (ipv4_header *)(packet.data + offset);
    proto = ipv4->proto;
    
	if (router_ != 0)
		router_->transmitPacket(proto, packet, offset + ipv4->size());

	return ProcessingStatus::Accepted;
}

Protocol IpProcessor::getProtocol()
{
    return Protocol::IP;
}

void IpProcessor::setRouter(IRouter * router_)
{
	this->router_ = router_;
}

IRouter * IpProcessor::router()
{
	return router_;
}