#ifndef PROCESSORPLUGIN_H
#define PROCESSORPLUGIN_H

#include "CommonInclude.h"
#include "IProcessorPlugin.h"

namespace DiplomBukov
{
    class ProcessorPlugin : public IProcessorPlugin
    {
        IProcessorCreator * processorCreator;

    public:
        ProcessorPlugin(IProcessorCreator * creator = 0);

        virtual IProcessorCreator * getProcessorCreator();
        virtual void setProcessorCreator(IProcessorCreator * creator) = 0;
    };
    // class ProcessorPlugin
}
// namespace DiplomBukov

#endif // PROCESSORPLUGIN_H