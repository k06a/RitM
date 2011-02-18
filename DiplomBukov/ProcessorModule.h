#ifndef PROCESSORMODULE_H
#define PROCESSORMODULE_H

#include "CommonInclude.h"
#include "IProcessorModule.h"

namespace DiplomBukov
{
    class ProcessorModule : public IProcessorModule
    {
        IProcessor * baseProcessor;

    public:
        ProcessorModule(IProcessor * processor = NULL);

        virtual IProcessor * getBaseProcessor();
        virtual void setBaseProcessor(IProcessor * processor);
    };
    // class ProcessorModule
}
// namespace DiplomBukov

#endif // PROCESSORMODULE_H