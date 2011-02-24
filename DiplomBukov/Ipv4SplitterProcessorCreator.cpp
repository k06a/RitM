#include "Ipv4SplitterProcessorCreator.h"
#include "Ipv4SplitterProcessor.h"

using namespace DiplomBukov;

IProcessorPtr Ipv4SplitterProcessorCreator::createProcessor(IRouterPtr router) const
{
    return IProcessorPtr(new Ipv4SplitterProcessor(router));
}