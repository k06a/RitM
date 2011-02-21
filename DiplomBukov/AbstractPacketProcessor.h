#ifndef ABSTRACTPACKETPROCESSOR_H
#define ABSTRACTPACKETPROCESSOR_H

#include "CommonInclude.h"
#include "IPacketProcessor.h"

namespace DiplomBukov
{
    class AbstractPacketProcessor : public virtual IPacketProcessor
    {
    protected:
        IPacketProcessor * prevProcessor;
        IPacketProcessor * nextProcessor;

    public:
        AbstractPacketProcessor::AbstractPacketProcessor()
            : prevProcessor(NULL), nextProcessor(NULL)
        {
        }

        virtual IPacketProcessor * getPointer()
        {
            return this;
        }

        virtual void ping(IPacketProcessor * prevProcessor)
        {
            setPrevProcessor(prevProcessor);
            prevProcessor->ping(this);
        }

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset)
        {
            if (nextProcessor != NULL)
                return nextProcessor->forwardProcess(proto, packet, offset);
            return ProcessingStatus::Rejected;
        }

        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset)
        {
            if (prevProcessor != NULL)
                return prevProcessor->backwardProcess(proto, packet, offset);
            return ProcessingStatus::Rejected;
        }

        virtual void setNextProcessor(IPacketProcessor * processor)
        {
            nextProcessor = processor;
        }

        virtual IPacketProcessor * getNextProcessor()
        {
            return nextProcessor;
        }

        virtual void setPrevProcessor(IPacketProcessor * processor)
        {
            prevProcessor = processor;
        }

        virtual IPacketProcessor * getPrevProcessor()
        {
            return prevProcessor;
        }
    };
    // class AbstractPacketProcessor
}
// namespace DiplomBukov

#endif // ABSTRACTPACKETPROCESSOR_H