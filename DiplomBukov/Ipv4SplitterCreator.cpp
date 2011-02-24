#include "Ipv4SplitterCreator.h"
#include "Ipv4Splitter.h"

using namespace DiplomBukov;

IProcessorPtr Ipv4SplitterCreator::createProcessor(IRouterPtr router) const
{
    return IProcessorPtr(new Ipv4Splitter(router));
}