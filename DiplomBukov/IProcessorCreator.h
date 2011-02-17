#ifndef IPROCESSORCREATOR_H
#define IPROCESSORCREATOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IProcessorCreator
    {
    public:
        virtual ~IProcessorCreator() {}

        virtual IProcessor * createProcessor(IRouter * router = NULL) = 0;
    };
    // class IProcessorCreator
}
// namespace DiplomBukov

#endif // IPROCESSORCREATOR_H