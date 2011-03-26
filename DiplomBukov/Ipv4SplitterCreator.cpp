#include "Ipv4SplitterCreator.h"
#include "Ipv4Splitter.h"

using namespace DiplomBukov;

ProcessorPtr Ipv4SplitterCreator::createProcessor(ConnectorPtr Connector) const
{
    return ProcessorPtr(new Ipv4Splitter(Connector));
}