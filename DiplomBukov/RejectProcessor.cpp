#include "RejectProcessor.h"

using namespace DiplomBukov;

RejectProcessor::RejectProcessor(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

ProcessorPtr RejectProcessor::CreateCopy() const
{
    ProcessorPtr ptr(new RejectProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
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