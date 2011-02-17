#ifndef PROCESSORPLUGIN_H
#define PROCESSORPLUGIN_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class ProcessorPlugin
    {
    public:
        virtual ~ProcessorPlugin() {}

        virtual IProcessorCreator * processorCreator() = 0;
        virtual void setProcessorCreator(IProcessorCreator * processorCreator) = 0;
    };
    // class ProcessorPlugin
}
// namespace DiplomBukov

#endif // PROCESSORPLUGIN_H