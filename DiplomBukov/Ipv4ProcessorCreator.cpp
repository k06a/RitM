#include "Ipv4ProcessorCreator.h"
#include "Ipv4Processor.h"

using namespace DiplomBukov;

IProcessor * Ipv4ProcessorCreator::createProcessor(IRouter * router)
{
    return new Ipv4Processor(router);
}