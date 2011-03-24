#include "DnsMessageProcessor.h"
#include "IPacket.h"
#include <algorithm>

using namespace DiplomBukov;

DnsMessageProcessor::DnsMessageProcessor(IProcessorPtr processor)
{
    setNextProcessor(processor);
}

IProcessorPtr DnsMessageProcessor::CreateCopy() const
{
    IProcessorPtr np = IProcessorPtr();
    if (nextProcessor != NULL)
        nextProcessor->CreateCopy();

    IProcessorPtr ptr(new DnsMessageProcessor(np));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus DnsMessageProcessor::forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    dnsMessage.Init(&packet->data()[offset], packet->size()-offset);

    backwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus DnsMessageProcessor::backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset)
{
    if (packet->processorBefore(Self) != NULL)
        packet->processorBefore(Self)->backwardProcess(proto, packet, offset);
    return ProcessingStatus::Accepted;
}

Protocol DnsMessageProcessor::getProtocol()
{
    return Protocol("UDP_53", 53);
}

const char * DnsMessageProcessor::getProcessorName()
{
    return "DnsMessageProcessor";
}