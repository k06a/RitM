#ifndef Ipv4SplitterCREATOR_H
#define Ipv4SplitterCREATOR_H

#include "IConnector.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class Ipv4SplitterCreator : public IProcessorCreator
    {
    public:
        virtual ProcessorPtr createProcessor(ConnectorPtr Connector = ConnectorPtr()) const;
    };
    // class MacProcessorCreator
}
// namespace DiplomBukov

#endif // Ipv4SplitterCREATOR_H