#include "ProcessorModule.h"
#include "IProcessor.h"

using namespace DiplomBukov;

ProcessorModule::ProcessorModule(IProcessor * baseProcessor)
    : baseProcessor(baseProcessor)
{
    if (baseProcessor != NULL)
        baseProcessor->setModule(this);
}

IProcessor * ProcessorModule::getBaseProcessor()
{
    return baseProcessor;
}

void ProcessorModule::setBaseProcessor(IProcessor * baseProcessor)
{
    this->baseProcessor = baseProcessor;
    if (baseProcessor != NULL)
        baseProcessor->setModule(this);
}