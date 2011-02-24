#ifndef ROUTERMODULE_H
#define ROUTERMODULE_H

#include "CommonInclude.h"
#include "IRouterModule.h"

namespace DiplomBukov
{
    class RouterModule : public IRouterModule
    {
        IRouterPtr baseRouter;

    public:
        RouterModule(IRouterPtr router = IRouterPtr());

        virtual IRouterPtr getBaseRouter();
        virtual void setBaseRouter(IRouterPtr router);
    };
    // class RouterModule
}
// namespace DiplomBukov

#endif // ROUTERMODULE_H