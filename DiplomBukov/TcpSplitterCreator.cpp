#include "TcpSplitterCreator.h"
#include "TcpSplitter.h"

using namespace DiplomBukov;

ProcessorPtr TcpSplitterCreator::createProcessor(ConnectorPtr Connector) const
{
    return ProcessorPtr(new TcpSplitter(Connector));
}