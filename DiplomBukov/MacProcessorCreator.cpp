#include "MacProcessorCreator.h"
#include "MacProcessor.h"

using namespace DiplomBukov;

IProcessor * MacProcessorCreator::createProcessor(IRouter * router)
{
    return new MacProcessor(router);
}