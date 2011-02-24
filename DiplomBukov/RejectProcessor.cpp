#include "RejectProcessor.h"

using namespace DiplomBukov;

RejectProcessor::RejectProcessor(IProcessorPtr router)
{
    setNextProcessor(router);
}

IProcessorPtr RejectProcessor::CreateCopy() const
{
    return IProcessorPtr(new RejectProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus RejectProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    packet->setStatus(IPacket::Rejected);

    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

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