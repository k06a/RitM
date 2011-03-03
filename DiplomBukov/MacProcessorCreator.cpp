#include "MacProcessorCreator.h"
#include "MacHeaderProcessor.h"

using namespace DiplomBukov;

IProcessorPtr MacProcessorCreator::createProcessor(IConnectorPtr Connector) const
{
    return IProcessorPtr(new MacHeaderProcessor(Connector));
}