#include "ProcessorPlugin.h"

using namespace DiplomBukov;

ProcessorPlugin::ProcessorPlugin(IProcessorCreator * creator)
    : processorCreator(creator)
{

}

IProcessorCreator * ProcessorPlugin::getProcessorCreator()
{
    return processorCreator;
}

void ProcessorPlugin::setProcessorCreator(IProcessorCreator * creator)
{
    processorCreator = creator;
}