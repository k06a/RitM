#include "ProcessorModule.h"

using namespace DiplomBukov;

ProcessorModule::ProcessorModule(IProcessor * processor)
    : baseProcessor_(processor)
{
}

IProcessor * ProcessorModule::baseProcessor()
{
    return baseProcessor_;
}

void ProcessorModule::setBaseProcessor(IProcessor * processor)
{
    baseProcessor_ = processor;
}