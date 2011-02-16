#ifndef IPROCESSORCREATOR_H
#define IPROCESSORCREATOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IRouter;
    class IAdapter;
    class IProcessor;
    class IRouterCreator;
    class IProcessorCreator;

    class IProcessorCreator
    {
    public:
        virtual ~IProcessorCreator() {}

        virtual IProcessor * createProcessor(IRouter * router = NULL) = 0;
        virtual IProcessor * createProcessorCopy(IProcessor * processor = NULL) = 0;
    };
    // class IProcessorCreator
}
// namespace DiplomBukov

#endif // IPROCESSORCREATOR_H