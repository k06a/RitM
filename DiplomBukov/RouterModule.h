#ifndef ROUTERMODULE_H
#define ROUTERMODULE_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class RouterModule
    {
        IRouter * baseRouter_;

    public:
        RouterModule(IRouter * router = NULL);

        IRouter * baseRouter();
        void setBaseRouter(IRouter * router);
    };
    // class RouterModule
}
// namespace DiplomBukov

#endif // ROUTERMODULE_H