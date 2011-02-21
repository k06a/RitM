#include "Ipv4SplitterProcessor.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

Ipv4SplitterProcessor::Ipv4SplitterProcessor(IPacketProcessor * router)
{
    setNextProcessor(router);
}

IProcessor * Ipv4SplitterProcessor::CreateCopy() const
{
    return new Ipv4SplitterProcessor(nextProcessor->CreateCopy());
}

ProcessingStatus Ipv4SplitterProcessor::forwardProcess(Protocol proto, Packet & packet, unsigned offset)
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
        if (nextProcessor != NULL)
            routers[para] = nextProcessor->CreateCopy();
    }

    if (nextProcessor != NULL)
        routers[para]->forwardProcess(proto, packet, offset);

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
