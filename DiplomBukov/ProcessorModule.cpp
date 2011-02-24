#include "ProcessorModule.h"
#include "IProcessor.h"

using namespace DiplomBukov;

ProcessorModule::ProcessorModule(IProcessorPtr baseProcessor)
    : baseProcessor(baseProcessor)
{
    if (baseProcessor != NULL)
        baseProcessor->setModule(this);
}

IProcessorPtr ProcessorModule::getBaseProcessor()
{
    return baseProcessor;
}

void ProcessorModule::setBaseProcessor(IProcessorPtr baseProcessor)
{
    this->baseProcessor = baseProcessor;
    if (baseProcessor != NULL)
        baseProcessor->setModule(this);
}