#include "TcpSplitterCreator.h"
#include "TcpSplitter.h"

using namespace DiplomBukov;

IProcessorPtr TcpSplitterCreator::createProcessor(IRouterPtr router) const
{
    return IProcessorPtr(new TcpSplitter(router));
}