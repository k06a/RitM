#include "IcmpHeaderProcessor.h"
#include "network/icmp_header.h"

using namespace RitM;

IcmpHeaderProcessor::IcmpHeaderProcessor(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

ProcessorPtr IcmpHeaderProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new IcmpHeaderProcessor(np));
}

ProcessingStatus IcmpHeaderProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(shared_from_this());

    icmp_header * icmp = (icmp_header *)(&(*packet)[offset]);

    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(Protocol::None, packet, offset + sizeof(icmp_header));

    return ProcessingStatus::Accepted;
}

const char * IcmpHeaderProcessor::getProcessorName()
{
    return "IcmpHeaderProcessor";
}

Protocol IcmpHeaderProcessor::getProtocol()
{
    return Protocol::ICMP;
}