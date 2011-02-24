#include "Ipv4Processor.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

Ipv4Processor::Ipv4Processor(IProcessorPtr router)
{
    setNextProcessor(router);
}

IProcessorPtr Ipv4Processor::CreateCopy() const
{
    return IProcessorPtr(new Ipv4Processor(nextProcessor->CreateCopy()));
}

ProcessingStatus Ipv4Processor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    ipv4_header * ip = (ipv4_header *)(packet->data() + offset);
    if (ip->version != 4)
        return ProcessingStatus::Rejected;
    
    packet->addProcessor(Self);

    //

    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(ip->proto, packet, offset + ip->size());

    return ProcessingStatus::Accepted;
}

const char * Ipv4Processor::getProcessorName()
{
    return "Ipv4Processor";
}

Protocol Ipv4Processor::getProtocol()
{
    return Protocol::IPv4;
}
