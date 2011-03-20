#include "RejectProcessor.h"

using namespace DiplomBukov;

RejectProcessor::RejectProcessor(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr RejectProcessor::CreateCopy() const
{
    IProcessorPtr ptr(new RejectProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus RejectProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
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