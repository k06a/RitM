#include "MacProcessorCreator.h"
#include "MacHeaderProcessor.h"

using namespace DiplomBukov;

ProcessorPtr MacProcessorCreator::createProcessor(ConnectorPtr Connector) const
{
    return ProcessorPtr(new MacHeaderProcessor(Connector));
}