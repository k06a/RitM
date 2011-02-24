#include "Ipv4Splitter.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

Ipv4Splitter::Ipv4Splitter(IProcessorPtr router)
{
    setNextProcessor(router);
}

IProcessorPtr Ipv4Splitter::CreateCopy() const
{
    return IProcessorPtr(new Ipv4Splitter(nextProcessor->CreateCopy()));
}

ProcessingStatus Ipv4Splitter::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    ipv4_header * ipv4 = (ipv4_header *)(packet->data() + offset);
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

Protocol Ipv4Splitter::getProtocol()
{
    return Protocol::IPv4;
}

const char * Ipv4Splitter::getProcessorName()
{
    return "Ipv4Splitter";
}
