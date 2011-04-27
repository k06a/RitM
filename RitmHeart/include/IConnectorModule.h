#ifndef ICONNECTORMODULE_H
#define ICONNECTORMODULE_H

#include "CommonInclude.h"
#include "IModule.h"

namespace DiplomBukov
{
    class IConnectorModule : public IModule
    {
    public:
        virtual ~IConnectorModule() {}
        
        virtual ConnectorPtr createConnector() const = 0;
    };
    // class IConnectorModule
}
// namespace DiplomBukov

#endif // ICONNECTORMODULE_H