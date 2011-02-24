#ifndef IProcessorCreator_H
#define IProcessorCreator_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IProcessorCreator
    {
    public:
        virtual ~IProcessorCreator() {}

        virtual IProcessorPtr createProcessor(IRouterPtr router) const = 0;
    };
    // class IProcessorCreator
}
// namespace DiplomBukov

#endif // IProcessorCreator_H