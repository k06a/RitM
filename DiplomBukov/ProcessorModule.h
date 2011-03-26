#ifndef PROCESSORMODULE_H
#define PROCESSORMODULE_H

#include "CommonInclude.h"
#include "IProcessorModule.h"

namespace DiplomBukov
{
    class ProcessorModule : public IProcessorModule
    {
        ProcessorPtr baseProcessor;

    public:
        ProcessorModule(ProcessorPtr processor = ProcessorPtr());

        virtual ProcessorPtr getBaseProcessor();
        virtual void setBaseProcessor(ProcessorPtr processor);
    };
    // class ProcessorModule
}
// namespace DiplomBukov

#endif // PROCESSORMODULE_H