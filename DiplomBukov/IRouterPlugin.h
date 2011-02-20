#ifndef IROUTERPLUGIN_H
#define IROUTERPLUGIN_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IRouterPlugin
    {
    public:
        virtual ~IRouterPlugin() {}

        virtual IRouterCreator * getRouterCreator() = 0;
        virtual void setRouterCreator(IRouterCreator * creator) = 0;
    };
    // class IRouterPlugin
}
// namespace DiplomBukov

#endif // IROUTERPLUGIN_H