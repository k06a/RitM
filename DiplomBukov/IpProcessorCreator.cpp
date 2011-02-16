#include "IpProcessorCreator.h"
#include "IpProcessor.h"

using namespace DiplomBukov;

IProcessor * IpProcessorCreator::createProcessor(IRouter * router)
{
    return new IpProcessor(router);
}