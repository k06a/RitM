#ifndef ABSTRACTPROCESSOR_H
#define ABSTRACTPROCESSOR_H

#include "CommonInclude.h"
#include "IProcessor.h"

namespace DiplomBukov
{
    class RITM_API AbstractProcessor
        : public EnableSharedFromThis<IProcessor>::Type
        , public virtual IProcessor
    {
    protected:
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

        virtual void setNextProcessor(ProcessorPtr processor);
        virtual ProcessorPtr getNextProcessor();

        virtual void setPrevProcessor(ProcessorPtr processor);
        virtual ProcessorPtr getPrevProcessor();

        virtual void setExemplar(IProcessorModule * mod);
        virtual IProcessorModule * getExemplar();

        virtual OptionPtr getOptions();

        virtual StatsProviderPtr statsProvider();

        virtual bool isFinal();
    };
    // class AbstractProcessor
}
// namespace DiplomBukov

#endif // ABSTRACTPROCESSOR_H