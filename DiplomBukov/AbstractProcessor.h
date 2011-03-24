#ifndef ABSTRACTPROCESSOR_H
#define ABSTRACTPROCESSOR_H

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
        AbstractProcessor();
        virtual IProcessorPtr CreateCopy() const;
        virtual void DestroyHierarchy();

        virtual IProcessorPtr getPointer();
        virtual void ping(IProcessorPtr prevProcessor);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual void setSelf(IProcessorPtr proc);
        virtual IProcessorPtr self();

        virtual void setNextProcessor(IProcessorPtr processor);
        virtual IProcessorPtr getNextProcessor();

        virtual void setPrevProcessor(IProcessorPtr processor);
        virtual IProcessorPtr getPrevProcessor();

        virtual void setModule(IProcessorModule * mod);
        virtual IProcessorModule * getModule();

        virtual std::deque<IOptionPtr> getOptions();
    };
    // class AbstractProcessor
}
// namespace DiplomBukov

#endif // ABSTRACTPROCESSOR_H