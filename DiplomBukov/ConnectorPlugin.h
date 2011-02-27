#ifndef CONNECTORPLUGIN_H
#define CONNECTORPLUGIN_H

#include "CommonInclude.h"
#include "IConnectorPlugin.h"

namespace DiplomBukov
{
    class ConnectorPlugin : public IConnectorPlugin
    {
        IConnectorCreator * ConnectorCreator;

    public:
        ConnectorPlugin(IConnectorCreator * creator = 0);

        virtual IConnectorCreator * getConnectorCreator();
        virtual void setConnectorCreator(IConnectorCreator * creator);
    };
    // class ConnectorPlugin
}
// namespace DiplomBukov

#endif // CONNECTORPLUGIN_H