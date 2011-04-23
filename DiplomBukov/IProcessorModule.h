#ifndef IProcessorModule_H
#define IProcessorModule_H

#include "CommonInclude.h"
#include "IModule.h"

namespace DiplomBukov
{
    class IProcessorModule : public IModule
    {
    public:
        virtual ~IProcessorModule() {}
        
        virtual ProcessorPtr createProcessor() const = 0;
    };
    // class IProcessorModule
}
// namespace DiplomBukov

#endif // IProcessorModule_H