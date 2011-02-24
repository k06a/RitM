#include "MacProcessorCreator.h"
#include "MacProcessor.h"

using namespace DiplomBukov;

IProcessorPtr MacProcessorCreator::createProcessor(IRouterPtr router) const
{
    return IProcessorPtr(new MacProcessor(router));
}