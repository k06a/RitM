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
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new AcceptProcessor(np));
}

ProcessingStatus AcceptProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    packet->setStatus(IPacket::Accepted);

    packet->addProcessor(shared_from_this());
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

bool AcceptProcessor::isFinal()
{
    return true;
}