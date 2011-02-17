#include "Ipv4SplitterProcessorCreator.h"
#include "Ipv4SplitterProcessor.h"

using namespace DiplomBukov;

IProcessor * Ipv4SplitterProcessorCreator::createProcessor(IRouter * router)
{
    return new Ipv4SplitterProcessor(router);
}