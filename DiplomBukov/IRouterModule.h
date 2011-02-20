#ifndef IROUTERMODULE_H
#define IROUTERMODULE_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IRouterModule
    {
    public:
        virtual ~IRouterModule() {}
        
        virtual IRouter * getBaseRouter() = 0;
        virtual void setBaseRouter(IRouter * router) = 0;
    };
    // class IRouterModule
}
// namespace DiplomBukov

#endif // IROUTERMODULE_H