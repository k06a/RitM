#ifndef CONNECTORMODULE_H
#define CONNECTORMODULE_H

#include "CommonInclude.h"
#include "IConnectorModule.h"

namespace DiplomBukov
{
    class ConnectorModule : public IConnectorModule
    {
        IConnectorPtr baseConnector;

    public:
        ConnectorModule(IConnectorPtr Connector = IConnectorPtr());

        virtual IConnectorPtr getBaseConnector();
        virtual void setBaseConnector(IConnectorPtr Connector);
    };
    // class ConnectorModule
}
// namespace DiplomBukov

#endif // CONNECTORMODULE_H