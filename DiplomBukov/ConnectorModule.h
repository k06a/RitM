#ifndef CONNECTORMODULE_H
#define CONNECTORMODULE_H

#include "CommonInclude.h"
#include "IConnectorModule.h"

namespace DiplomBukov
{
    class ConnectorModule : public IConnectorModule
    {
        ConnectorPtr baseConnector;

    public:
        ConnectorModule(ConnectorPtr Connector = ConnectorPtr());

        virtual ConnectorPtr getBaseConnector();
        virtual void setBaseConnector(ConnectorPtr Connector);
    };
    // class ConnectorModule
}
// namespace DiplomBukov

#endif // CONNECTORMODULE_H