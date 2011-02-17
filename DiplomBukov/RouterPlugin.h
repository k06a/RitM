#ifndef ROUTERPLUGIN_H
#define ROUTERPLUGIN_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class RouterPlugin
    {
    public:
        virtual ~RouterPlugin() {}

        virtual IRouterCreator * routerCreator() = 0;
        virtual void setRouterCreator(IRouterCreator * routerCreator) = 0;
    };
    // class RouterPlugin
}
// namespace DiplomBukov

#endif // ROUTERPLUGIN_H