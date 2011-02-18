#ifndef IPROCESSORMODULE_H
#define IPROCESSORMODULE_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IProcessorModule
    {
    public:
        virtual ~IProcessorModule() {}
        
        virtual IProcessor * getBaseProcessor() = 0;
        virtual void setBaseProcessor(IProcessor * processor) = 0;
    };
    // class IProcessorModule
}
// namespace DiplomBukov

#endif // IPROCESSORMODULE_H