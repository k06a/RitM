#include "AcceptProcessor.h"

using namespace DiplomBukov;

AcceptProcessor::AcceptProcessor(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr AcceptProcessor::CreateCopy() const
{
    return IProcessorPtr(new AcceptProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus AcceptProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    packet->setStatus(IPacket::Accepted);

    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

Protocol AcceptProcessor::getProtocol()
{
    return Protocol::None;
}

const char * AcceptProcessor::getProcessorName()
{
    return "AcceptProcessor";
}