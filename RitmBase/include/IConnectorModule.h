#ifndef ICONNECTORMODULE_H
#define ICONNECTORMODULE_H

#include "CommonInclude.h"
#include "IModule.h"

namespace RitM
{
    class IConnectorModule : public IModule
    {
    public:
        virtual ~IConnectorModule() {}
        
        virtual ConnectorPtr createConnector() const = 0;
    };
    // class IConnectorModule
}
// namespace RitM

#endif // ICONNECTORMODULE_H