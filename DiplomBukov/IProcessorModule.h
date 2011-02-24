#ifndef IProcessorModule_H
#define IProcessorModule_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IProcessorModule
    {
    public:
        virtual ~IProcessorModule() {}
        
        virtual IProcessorPtr getBaseProcessor() = 0;
        virtual void setBaseProcessor(IProcessorPtr processor) = 0;
    };
    // class IProcessorModule
}
// namespace DiplomBukov

#endif // IProcessorModule_H