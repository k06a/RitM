#ifndef IROUTERCREATOR_H
#define IROUTERCREATOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IRouter;
    class IAdapter;
    class IProcessor;
    class IRouterCreator;
    class IProcessorCreator;

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