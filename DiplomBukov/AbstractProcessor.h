#ifndef ABSTRACTPROCESSOR_H
#define ABSTRACTPROCESSOR_H

#include "CommonInclude.h"
#include "IProcessor.h"
#include "AbstractPacketProcessor.h"

namespace DiplomBukov
{
    class AbstractProcessor
        : public AbstractPacketProcessor
        , public IProcessor
    {
    protected:
        IProcessorModule * module;

    public:
        AbstractProcessor::AbstractProcessor()
            : module(NULL)
        {
        }

        virtual void setModule(IProcessorModule * mod)
        {
            module = mod;
        }

        virtual IProcessorModule * getModule()
        {
            return module;
        }

        virtual Protocol getProtocol()
        {
            return Protocol::None;
        }

        virtual const char * getProcessorName()
        {
            return "EMPTY";
        }
    };
    // class AbstractProcessor
}
// namespace DiplomBukov

#endif // ABSTRACTPROCESSOR_H