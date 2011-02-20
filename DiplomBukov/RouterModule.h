#ifndef ROUTERMODULE_H
#define ROUTERMODULE_H

#include "CommonInclude.h"
#include "IRouterModule.h"

namespace DiplomBukov
{
    class RouterModule : public IRouterModule
    {
        IRouter * baseRouter;

    public:
        RouterModule(IRouter * router = NULL);

        virtual IRouter * getBaseRouter();
        virtual void setBaseRouter(IRouter * router);
    };
    // class RouterModule
}
// namespace DiplomBukov

#endif // ROUTERMODULE_H