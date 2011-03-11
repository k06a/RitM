#ifndef AbstractProcessor_H
#define AbstractProcessor_H

#include "CommonInclude.h"
#include "IProcessor.h"

namespace DiplomBukov
{
    class AbstractProcessor : public virtual IProcessor
    {
    protected:
        IProcessorPtr Self;
        IProcessorPtr prevProcessor;
        IProcessorPtr nextProcessor;
        IProcessorModule * module;

    public:
        AbstractProcessor::AbstractProcessor()
            : Self(), prevProcessor()
            , nextProcessor(), module()
        {
        }

        virtual IProcessorPtr getPointer()
        {
            return Self;
        }

        virtual void ping(IProcessorPtr prevProcessor)
        {
            setPrevProcessor(prevProcessor);
            prevProcessor->ping(Self);
        }

        virtual Protocol getProtocol()
        {
            return Protocol::None;
        }

        virtual const char * getProcessorName()
        {
            return "EMPTY";
        }

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
        {
            if (nextProcessor != NULL)
                return nextProcessor->forwardProcess(proto, packet, offset);
            return ProcessingStatus::Rejected;
        }

        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
        {
            if (prevProcessor != NULL)
                return prevProcessor->backwardProcess(proto, packet, offset);
            return ProcessingStatus::Rejected;
        }

        virtual void setSelf(IProcessorPtr proc)
        {
            Self = proc;
        }

        virtual IProcessorPtr self()
        {
            return Self;
        }

        virtual void setNextProcessor(IProcessorPtr processor)
        {
            nextProcessor = processor;
        }

        virtual IProcessorPtr getNextProcessor()
        {
            return nextProcessor;
        }

        virtual void setPrevProcessor(IProcessorPtr processor)
        {
            prevProcessor = processor;
        }

        virtual IProcessorPtr getPrevProcessor()
        {
            return prevProcessor;
        }

        virtual void setModule(IProcessorModule * mod)
        {
            module = mod;
        }

        virtual IProcessorModule * getModule()
        {
            return module;
        }

        virtual std::deque<IOptionPtr> getOptions()
        {
            return std::deque<IOptionPtr>();
        }
    };
    // class AbstractProcessor
}
// namespace DiplomBukov

#endif // AbstractProcessor_H