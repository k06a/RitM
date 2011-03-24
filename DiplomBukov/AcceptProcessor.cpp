#include "AcceptProcessor.h"

using namespace DiplomBukov;

AcceptProcessor::AcceptProcessor(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr AcceptProcessor::CreateCopy() const
{
    IProcessorPtr np = IProcessorPtr();
    if (nextProcessor != NULL)
        nextProcessor->CreateCopy();

    IProcessorPtr ptr(new AcceptProcessor(np));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus AcceptProcessor::forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset)
{
    packet->setStatus(IPacket::Accepted);

    packet->addProcessor(Self);
    backwardProcess(proto, packet, offset);
    
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