#include "AcceptProcessor.h"

using namespace DiplomBukov;

AcceptProcessor::AcceptProcessor(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

ProcessorPtr AcceptProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        nextProcessor->CreateCopy();

    ProcessorPtr ptr(new AcceptProcessor(np));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus AcceptProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
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