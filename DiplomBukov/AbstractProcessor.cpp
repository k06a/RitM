#include "AbstractProcessor.h"
#include "IPacket.h"

using namespace DiplomBukov;

AbstractProcessor::AbstractProcessor()
    : Self(), prevProcessor()
    , nextProcessor(), module()
{
}

ProcessorPtr AbstractProcessor::CreateCopy() const
{
    if (this == NULL)
        return ProcessorPtr();
    else
        throw "Not Implemented";
}

void AbstractProcessor::DestroyHierarchy()
{
    if (nextProcessor != NULL)
        nextProcessor->DestroyHierarchy();
    setPrevProcessor(ProcessorPtr());
    setNextProcessor(ProcessorPtr());
    setSelf(ProcessorPtr());
}

ProcessorPtr AbstractProcessor::getPointer()
{
    return Self;
}

void AbstractProcessor::ping(ProcessorPtr prevProcessor)
{
    setPrevProcessor(prevProcessor);
    if (nextProcessor != NULL)
        nextProcessor->ping(Self);
}

Protocol AbstractProcessor::getProtocol()
{
    return Protocol::None;
}

const char * AbstractProcessor::getProcessorName()
{
    return "EMPTY";
}

ProcessingStatus AbstractProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (nextProcessor != NULL)
        return nextProcessor->forwardProcess(proto, packet, offset);
    return ProcessingStatus::Rejected;
}

ProcessingStatus AbstractProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (packet->processorBefore(Self) != NULL)
        return packet->processorBefore(Self)->backwardProcess(proto, packet, offset);
    return ProcessingStatus::Rejected;
}

void AbstractProcessor::setSelf(ProcessorPtr proc)
{
    Self = proc;
}

ProcessorPtr AbstractProcessor::self()
{
    return Self;
}

void AbstractProcessor::setNextProcessor(ProcessorPtr processor)
{
    nextProcessor = processor;
}

ProcessorPtr AbstractProcessor::getNextProcessor()
{
    return nextProcessor;
}

void AbstractProcessor::setPrevProcessor(ProcessorPtr processor)
{
    prevProcessor = processor;
}

ProcessorPtr AbstractProcessor::getPrevProcessor()
{
    return prevProcessor;
}

void AbstractProcessor::setModule(IProcessorModule * mod)
{
    module = mod;
}

IProcessorModule * AbstractProcessor::getModule()
{
    return module;
}

OptionPtr AbstractProcessor::getOptions()
{
    return OptionPtr();
}