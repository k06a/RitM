#ifndef TCPSPLITTERPROCESSORCREATOR_H
#define TCPSPLITTERPROCESSORCREATOR_H

#include "IConnector.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class TcpSplitterCreator : public IProcessorCreator
    {
    public:
        virtual ProcessorPtr createProcessor(ConnectorPtr Connector = ConnectorPtr()) const;
    };
    // class TcpSplitterCreator
}
// namespace DiplomBukov

#endif // TCPSPLITTERPROCESSORCREATOR_H