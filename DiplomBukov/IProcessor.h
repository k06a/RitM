#ifndef IProcessor_H
#define IProcessor_H

#include <memory>
#include "CommonInclude.h"

namespace DiplomBukov
{
    class IProcessor
    {
    public:
        virtual ~IProcessor() {}

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset) = 0;
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset) = 0;

        virtual ProcessorPtr CreateCopy() const = 0;
        virtual void DestroyHierarchy() = 0;
        virtual ProcessorPtr getPointer() = 0;
        virtual void ping(ProcessorPtr prevProcessor) = 0;

        virtual Protocol getProtocol() = 0;
        virtual const char * getProcessorName() = 0;

        virtual void setNextProcessor(ProcessorPtr processor) = 0;
        virtual ProcessorPtr getNextProcessor() = 0;

        virtual void setPrevProcessor(ProcessorPtr processor) = 0;
        virtual ProcessorPtr getPrevProcessor() = 0;

        virtual void setModule(IProcessorModule * module) = 0;
        virtual IProcessorModule * getModule() = 0;

        virtual OptionPtr getOptions() = 0;
    };
    // class IProcessor
}
// namespace DiplomBukov

#endif // IProcessor_H