#ifndef ABSTRACTPROCESSOR_H
#define ABSTRACTPROCESSOR_H

#include "CommonInclude.h"
#include "IProcessor.h"

namespace DiplomBukov
{
    class AbstractProcessor : public virtual IProcessor
    {
    protected:
        ProcessorPtr Self;
        ProcessorPtr prevProcessor;
        ProcessorPtr nextProcessor;
        IProcessorModule * module;

    public:
        AbstractProcessor();
        virtual ProcessorPtr CreateCopy() const;
        virtual void DestroyHierarchy();

        virtual ProcessorPtr getPointer();
        virtual void ping(ProcessorPtr prevProcessor);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual void setSelf(ProcessorPtr proc);
        virtual ProcessorPtr self();

        virtual void setNextProcessor(ProcessorPtr processor);
        virtual ProcessorPtr getNextProcessor();

        virtual void setPrevProcessor(ProcessorPtr processor);
        virtual ProcessorPtr getPrevProcessor();

        virtual void setModule(IProcessorModule * mod);
        virtual IProcessorModule * getModule();

        virtual OptionPtr getOptions();
    };
    // class AbstractProcessor
}
// namespace DiplomBukov

#endif // ABSTRACTPROCESSOR_H