#include "TcpSplitterProcessorCreator.h"
#include "TcpSplitterProcessor.h"

using namespace DiplomBukov;

IProcessor * TcpSplitterProcessorCreator::createProcessor(IRouter * router)
{
    return new TcpSplitterProcessor(router);
}