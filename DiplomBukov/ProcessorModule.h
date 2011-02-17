#ifndef PROCESSORMODULE_H
#define PROCESSORMODULE_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class ProcessorModule
    {
        IProcessor * baseProcessor_;

    public:
        ProcessorModule(IProcessor * processor = NULL);

        IProcessor * baseProcessor();
        void setBaseProcessor(IProcessor * processor);
    };
    // class ProcessorModule
}
// namespace DiplomBukov

#endif // PROCESSORMODULE_H