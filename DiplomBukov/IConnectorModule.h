#ifndef ICONNECTORMODULE_H
#define ICONNECTORMODULE_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IConnectorModule
    {
    public:
        virtual ~IConnectorModule() {}
        
        virtual IConnectorPtr getBaseConnector() = 0;
        virtual void setBaseConnector(IConnectorPtr Connector) = 0;
    };
    // class IConnectorModule
}
// namespace DiplomBukov

#endif // ICONNECTORMODULE_H