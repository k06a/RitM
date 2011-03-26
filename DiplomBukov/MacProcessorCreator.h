#ifndef MACPROCESSORCREATOR_H
#define MACPROCESSORCREATOR_H

#include "IConnector.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class MacProcessorCreator : public IProcessorCreator
    {
    public:
        virtual ProcessorPtr createProcessor(ConnectorPtr Connector = ConnectorPtr()) const;
    };
    // class MacProcessorCreator
}
// namespace DiplomBukov

#endif // MACPROCESSORCREATOR_H