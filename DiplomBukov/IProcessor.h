#ifndef IProcessor_H
#define IProcessor_H

#include <deque>
#include <memory>
#include "CommonInclude.h"

namespace DiplomBukov
{
    class IProcessor
    {
    public:
        virtual ~IProcessor() {}

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset) = 0;
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset) = 0;

        virtual IProcessorPtr CreateCopy() const = 0;
        virtual void DestroyHierarchy() = 0;
        virtual IProcessorPtr getPointer() = 0;
        virtual void ping(IProcessorPtr prevProcessor) = 0;

        virtual Protocol getProtocol() = 0;
        virtual const char * getProcessorName() = 0;

        virtual void setSelf(IProcessorPtr proc) = 0;
        virtual IProcessorPtr self() = 0;

        virtual void setNextProcessor(IProcessorPtr processor) = 0;
        virtual IProcessorPtr getNextProcessor() = 0;

        virtual void setPrevProcessor(IProcessorPtr processor) = 0;
        virtual IProcessorPtr getPrevProcessor() = 0;

        virtual void setModule(IProcessorModule * module) = 0;
        virtual IProcessorModule * getModule() = 0;

        //virtual void setOptions(std::deque<IOptionPtr>) = 0;
        virtual std::deque<IOptionPtr> getOptions() = 0;
    };
    // class IProcessor
}
// namespace DiplomBukov

#endif // IProcessor_H