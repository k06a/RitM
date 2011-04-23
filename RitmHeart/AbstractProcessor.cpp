#include "AbstractProcessor.h"
#include "IPacket.h"

using namespace DiplomBukov;

AbstractProcessor::AbstractProcessor()
    : prevProcessor(), nextProcessor(), module()
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
}

ProcessorPtr AbstractProcessor::getPointer()
{
    return shared_from_this();
}

void AbstractProcessor::ping(ProcessorPtr prevProcessor)
{
    setPrevProcessor(prevProcessor);
    if (nextProcessor != NULL)
        nextProcessor->ping(shared_from_this());
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
    if (packet->processorBefore(shared_from_this()) != NULL)
        return packet->processorBefore(shared_from_this())->backwardProcess(proto, packet, offset);
    return ProcessingStatus::Rejected;
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

void AbstractProcessor::setExemplar(IProcessorModule * mod)
{
    module = mod;
}

IProcessorModule * AbstractProcessor::getExemplar()
{
    return module;
}

OptionPtr AbstractProcessor::getOptions()
{
    return OptionPtr();
}

StatsProviderPtr AbstractProcessor::statsProvider()
{
    return StatsProviderPtr();
}