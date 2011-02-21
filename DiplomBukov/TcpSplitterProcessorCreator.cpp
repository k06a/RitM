#include "TcpSplitterProcessorCreator.h"
#include "TcpSplitterProcessor.h"

using namespace DiplomBukov;

IProcessor * TcpSplitterProcessorCreator::createProcessor(IRouter * router) const
{
    return new TcpSplitterProcessor(router);
}