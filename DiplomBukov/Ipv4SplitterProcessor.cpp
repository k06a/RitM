#include "Ipv4SplitterProcessor.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

Ipv4SplitterProcessor::Ipv4SplitterProcessor(IRouter * baseRouter)
	: baseRouter(baseRouter)
{
}

IProcessor * Ipv4SplitterProcessor::CreateCopy() const
{
    return new Ipv4SplitterProcessor(baseRouter->CreateCopy());
}

ProcessingStatus Ipv4SplitterProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != Protocol::IPv4))
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

    if (baseRouter != NULL)
        routers[para]->transmitPacket(proto, packet, offset);

	return ProcessingStatus::Accepted;
}

Protocol Ipv4SplitterProcessor::getProtocol()
{
    return Protocol::IPv4;
}

void Ipv4SplitterProcessor::setRouter(IRouter * router)
{
	baseRouter = router;
}

IRouter * Ipv4SplitterProcessor::router()
{
	return baseRouter;
}