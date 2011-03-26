#include "IcmpProcessor.h"
#include "network/icmp_header.h"

using namespace DiplomBukov;

IcmpProcessor::IcmpProcessor(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

ProcessorPtr IcmpProcessor::CreateCopy() const
{
    ProcessorPtr ptr(new IcmpProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus IcmpProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    icmp_header * icmp = (icmp_header *)(&packet->data()[offset]);

    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(Protocol::None, packet, offset + sizeof(icmp_header));

    return ProcessingStatus::Accepted;
}

const char * IcmpProcessor::getProcessorName()
{
    return "IcmpProcessor";
}

Protocol IcmpProcessor::getProtocol()
{
    return Protocol::ICMP;
}