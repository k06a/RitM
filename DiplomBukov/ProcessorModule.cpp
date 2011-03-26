#include "ProcessorModule.h"
#include "IProcessor.h"

using namespace DiplomBukov;

ProcessorModule::ProcessorModule(ProcessorPtr baseProcessor)
    : baseProcessor(baseProcessor)
{
    if (baseProcessor != NULL)
        baseProcessor->setModule(this);
}

ProcessorPtr ProcessorModule::getBaseProcessor()
{
    return baseProcessor;
}

void ProcessorModule::setBaseProcessor(ProcessorPtr baseProcessor)
{
    this->baseProcessor = baseProcessor;
    if (baseProcessor != NULL)
        baseProcessor->setModule(this);
}