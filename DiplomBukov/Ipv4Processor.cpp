#include "Ipv4Processor.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

Ipv4Processor::Ipv4Processor(IRouter * baseRouter)
	: baseRouter(baseRouter)
{
}

IProcessor * Ipv4Processor::CreateCopy() const
{
    return new Ipv4Processor(baseRouter->CreateCopy());
}

ProcessingStatus Ipv4Processor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != Protocol::IP))
        return ProcessingStatus::Rejected;

    if (baseRouter == NULL)
        return ProcessingStatus::Rejected;

    ipv4_header * ipv4 = (ipv4_header *)(packet.data + offset);
    ipv4_addr adr1 = ipv4->src_data;
    ipv4_addr adr2 = ipv4->dst_data;
    if (adr2 < adr1) std::swap(adr1, adr2);

    // Create new router if needed
    ipv4_pair para(adr1,adr2);
    MyMap::iterator it = routers.find(para);
    if (it == routers.end())
        routers[para] = baseRouter->CreateCopy();

    routers[para]->transmitPacket(ipv4->proto, packet, offset);
	return ProcessingStatus::Accepted;
}

Protocol Ipv4Processor::getProtocol()
{
    return Protocol::IP;
}

void Ipv4Processor::setRouter(IRouter * router)
{
	baseRouter = router;
}

IRouter * Ipv4Processor::router()
{
	return baseRouter;
}