#ifndef ROUTERPLUGIN_H
#define ROUTERPLUGIN_H

#include "CommonInclude.h"
#include "IRouterPlugin.h"

namespace DiplomBukov
{
    class RouterPlugin : public IRouterPlugin
    {
        IRouterCreator * routerCreator;

    public:
        RouterPlugin(IRouterCreator * creator = 0);

        virtual IRouterCreator * getRouterCreator();
        virtual void setRouterCreator(IRouterCreator * creator);
    };
    // class RouterPlugin
}
// namespace DiplomBukov

#endif // ROUTERPLUGIN_H