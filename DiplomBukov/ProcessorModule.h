#ifndef PROCESSORMODULE_H
#define PROCESSORMODULE_H

#include "CommonInclude.h"
#include "IProcessorModule.h"

namespace DiplomBukov
{
    class ProcessorModule : public IProcessorModule
    {
        IProcessorPtr baseProcessor;

    public:
        ProcessorModule(IProcessorPtr processor = IProcessorPtr());

        virtual IProcessorPtr getBaseProcessor();
        virtual void setBaseProcessor(IProcessorPtr processor);
    };
    // class ProcessorModule
}
// namespace DiplomBukov

#endif // PROCESSORMODULE_H