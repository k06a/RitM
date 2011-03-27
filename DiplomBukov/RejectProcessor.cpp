#include "RejectProcessor.h"

using namespace DiplomBukov;

RejectProcessor::RejectProcessor(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

ProcessorPtr RejectProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new RejectProcessor(np));
}

ProcessingStatus RejectProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    packet->setStatus(IPacket::Rejected);

    return ProcessingStatus::Accepted;
}

Protocol RejectProcessor::getProtocol()
{
    return Protocol::None;
}

const char * RejectProcessor::getProcessorName()
{
    return "RejectProcessor";
}