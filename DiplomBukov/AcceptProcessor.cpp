#include "AcceptProcessor.h"

using namespace DiplomBukov;

AcceptProcessor::AcceptProcessor(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr AcceptProcessor::CreateCopy() const
{
    IProcessorPtr ptr(new AcceptProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus AcceptProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
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