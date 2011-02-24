#include "TcpSplitterProcessorCreator.h"
#include "TcpSplitterProcessor.h"

using namespace DiplomBukov;

IProcessorPtr TcpSplitterProcessorCreator::createProcessor(IRouterPtr router) const
{
    return IProcessorPtr(new TcpSplitterProcessor(router));
}