#ifndef IROUTERCREATOR_H
#define IROUTERCREATOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IRouterCreator
    {
    public:
        virtual ~IRouterCreator() {}

        virtual IRouter * createRouter() = 0;
    };
    // class IRouterCreator
}
// namespace DiplomBukov

#endif // IROUTERCREATOR_H