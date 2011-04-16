#ifndef IMODULE_H
#define IMODULE_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IModule
    {
    public:
        virtual ~IModule() {}

        virtual ProcessorPtr getBaseProcessor() = 0;
        virtual void setBaseProcessor(ProcessorPtr processor) = 0;

        virtual void addProcessorCopy(ProcessorPtr processor) = 0;
    };
    // class IModule
}
// namespace DiplomBukov

#endif // IMODULE_H