#include "Ipv4SplitterProcessor.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

Ipv4SplitterProcessor::Ipv4SplitterProcessor(IRouter * router)
	: module(NULL), router(router)
{
}

IProcessor * Ipv4SplitterProcessor::CreateCopy() const
{
    return new Ipv4SplitterProcessor(router->CreateCopy());
}

ProcessingStatus Ipv4SplitterProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    ipv4_header * ipv4 = (ipv4_header *)(packet.data + offset);
    ipv4_addr adr1 = ipv4->src_data;
    ipv4_addr adr2 = ipv4->dst_data;
    if (adr2 < adr1) std::swap(adr1, adr2);

    // Create new router if needed
    ipv4_pair para(adr1,adr2);
    MyMap::iterator it = routers.find(para);
    if (it == routers.end())
    {
        if (router != NULL)
            routers[para] = router->CreateCopy();
    }

    if (router != NULL)
        routers[para]->processPacket(proto, packet, offset);

	return ProcessingStatus::Accepted;
}

Protocol Ipv4SplitterProcessor::getProtocol()
{
    return Protocol::IPv4;
}

const char * Ipv4SplitterProcessor::getProcessorName()
{
    return "Ipv4SplitterProcessor";
}

void Ipv4SplitterProcessor::setRouter(IRouter * router)
{
	this->router = router;
}

IRouter * Ipv4SplitterProcessor::getRouter()
{
	return router;
}

void Ipv4SplitterProcessor::setModule(IProcessorModule * module)
{
    this->module = module;
}

IProcessorModule * Ipv4SplitterProcessor::getModule()
{
    return module;
}