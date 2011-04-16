#ifndef TIMEDSTARTER_H
#define TIMEDSTARTER_H

#include <list>
#include <vector>
#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IStarter.h"

namespace DiplomBukov
{
    class DelayInjectProcessor
        : public DiplomBukov::AbstractProcessor
    {
    public:
        std::list<Protocol> recvProto;
        std::list<PacketPtr> recvPacket;
        std::list<unsigned> recvOffset;

    public:
        ProcessorPtr CreateCopy() const
        {
            return ProcessorPtr(new DelayInjectProcessor());
        }

        ProcessingStatus forwardProcess(Protocol proto,
                                        PacketPtr packet,
                                        unsigned offset)
        {
            recvProto.push_back(proto);
            recvPacket.push_back(packet);
            recvOffset.push_back(offset);
            return ProcessingStatus::Accepted;
        }

        ProcessingStatus backwardProcess(Protocol proto,
                                         PacketPtr packet,
                                         unsigned offset)
        {
            if (prevProcessor != NULL)
                prevProcessor->backwardProcess(proto, packet, offset);
            return ProcessingStatus::Accepted;
        }

        void go()
        {
            if (recvProto.size() == 0)
                return;

            nextProcessor->forwardProcess(recvProto.front(),
                                          recvPacket.front(),
                                          recvOffset.front());

            recvProto.pop_front();
            recvPacket.pop_front();
            recvOffset.pop_front();
        }

        void pickUpTo(ProcessorPtr adap)
        {
            ProcessorPtr self = shared_from_this();

            self->setNextProcessor(adap->getNextProcessor());
            adap->getNextProcessor()->setPrevProcessor(self);
            adap->setNextProcessor(self);
            self->setPrevProcessor(adap);
        }
    };

    typedef SmartPointer<DelayInjectProcessor>::Type DelayInjectProcessorPtr;

    class TimedStarter : public IStarter
    {
        std::vector<AdapterPtr> adapters;
        std::vector<DelayInjectProcessorPtr> delays;

    public:
        virtual void addAdapter(AdapterPtr adapter);
        virtual void start();
    };
}
// namespace DiplomBukov

#endif // TIMEDSTARTER_H