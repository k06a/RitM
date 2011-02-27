#include "TcpSplitterCreator.h"
#include "TcpSplitter.h"

using namespace DiplomBukov;

IProcessorPtr TcpSplitterCreator::createProcessor(IConnectorPtr Connector) const
{
    return IProcessorPtr(new TcpSplitter(Connector));
}