#include "MacProcessorCreator.h"
#include "MacProcessor.h"

using namespace DiplomBukov;

IProcessorPtr MacProcessorCreator::createProcessor(IConnectorPtr Connector) const
{
    return IProcessorPtr(new MacProcessor(Connector));
}