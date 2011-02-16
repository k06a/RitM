#include "TcpProcessorCreator.h"
#include "TcpProcessor.h"

using namespace DiplomBukov;

IProcessor * TcpProcessorCreator::createProcessor(IRouter * router)
{
    return new TcpProcessor(router);
}