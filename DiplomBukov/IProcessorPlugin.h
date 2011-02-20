#ifndef IPROCESSORPLUGIN_H
#define IPROCESSORPLUGIN_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IProcessorPlugin
    {
    public:
        virtual ~IProcessorPlugin() {}

        virtual IProcessorCreator * getProcessorCreator() = 0;
        virtual void setProcessorCreator(IProcessorCreator * creator) = 0;
    };
    // class IProcessorPlugin
}
// namespace DiplomBukov

#endif // IPROCESSORPLUGIN_H