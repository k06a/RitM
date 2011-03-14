#include "AbstractProcessor.h"
#include "IPacket.h"

using namespace DiplomBukov;

AbstractProcessor::AbstractProcessor()
    : Self(), prevProcessor()
    , nextProcessor(), module()
{
}

IProcessorPtr AbstractProcessor::getPointer()
{
    return Self;
}

void AbstractProcessor::ping(IProcessorPtr prevProcessor)
{
    setPrevProcessor(prevProcessor);
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

ProcessingStatus AbstractProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (nextProcessor != NULL)
        return nextProcessor->forwardProcess(proto, packet, offset);
    return ProcessingStatus::Rejected;
}

ProcessingStatus AbstractProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (packet->prevProcessor(Self) != NULL)
        return packet->prevProcessor(Self)->backwardProcess(proto, packet, offset);
    return ProcessingStatus::Rejected;
}

void AbstractProcessor::setSelf(IProcessorPtr proc)
{
    Self = proc;
}

IProcessorPtr AbstractProcessor::self()
{
    return Self;
}

void AbstractProcessor::setNextProcessor(IProcessorPtr processor)
{
    nextProcessor = processor;
}

IProcessorPtr AbstractProcessor::getNextProcessor()
{
    return nextProcessor;
}

void AbstractProcessor::setPrevProcessor(IProcessorPtr processor)
{
    prevProcessor = processor;
}

IProcessorPtr AbstractProcessor::getPrevProcessor()
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

std::deque<IOptionPtr> AbstractProcessor::getOptions()
{
    return std::deque<IOptionPtr>();
}