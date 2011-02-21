#include "MacProcessorCreator.h"
#include "MacProcessor.h"

using namespace DiplomBukov;

IProcessor * MacProcessorCreator::createProcessor(IRouter * router) const
{
    return new MacProcessor(router);
}