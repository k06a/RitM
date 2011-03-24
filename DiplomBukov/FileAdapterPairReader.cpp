
#include <list>

#include "FileAdapterPairReader.h"
#include "CommonInclude.h"
#include "FileAdapter.h"
#include "IPacket.h"

using namespace DiplomBukov;

class DelayReaderProcessor
    : public DiplomBukov::AbstractProcessor
{
public:
    std::list<Protocol> recvProto;
    std::list<IPacketPtr> recvPacket;
    std::list<unsigned> recvOffset;

public:
    IProcessorPtr CreateCopy() const
    {
        return IProcessorPtr(new DelayReaderProcessor());
    }

    ProcessingStatus forwardProcess(Protocol proto,
                                    IPacketPtr packet,
                                    unsigned offset)
    {
        recvProto.push_back(proto);
        recvPacket.push_back(packet);
        recvOffset.push_back(offset);
        return ProcessingStatus::Accepted;
    }

    ProcessingStatus backwardProcess(Protocol proto,
                                     IPacketPtr packet,
                                     unsigned offset)
    {
        if (prevProcessor != NULL)
            prevProcessor->backwardProcess(proto, packet, offset);
        return ProcessingStatus::Accepted;
    }

    void go()
    {
        nextProcessor->forwardProcess(recvProto.front(),
                                      recvPacket.front(),
                                      recvOffset.front());
        recvProto.pop_front();
        recvPacket.pop_front();
        recvOffset.pop_front();
    }
};

void fileAdapterPairReader(IAdapterPtr ad1, IAdapterPtr ad2)
{
    DelayReaderProcessor * delay1 = new DelayReaderProcessor();
    DelayReaderProcessor * delay2 = new DelayReaderProcessor();
    IProcessorPtr proc1(delay1);
    IProcessorPtr proc2(delay2);

    proc1->setSelf(proc1);
    proc2->setSelf(proc2);

    proc1->setNextProcessor(ad1->getNextProcessor());
    proc2->setNextProcessor(ad2->getNextProcessor());
    ad1->getNextProcessor()->setPrevProcessor(proc1);
    ad2->getNextProcessor()->setPrevProcessor(proc2);
    ad1->setNextProcessor(proc1);
    ad2->setNextProcessor(proc2);
    proc1->setPrevProcessor(ad1);
    proc2->setPrevProcessor(ad2);

    while (true)
    {
        if (delay1->recvPacket.size() == 0)
            ad1->tick();
        if (delay2->recvPacket.size() == 0)
            ad2->tick();

        if ((delay1->recvPacket.size() == 0) && (delay2->recvPacket.size() == 0))
            break;

        if (delay1->recvPacket.size() == 0)
        {
            delay2->go();
            continue;
        }

        if (delay2->recvPacket.size() == 0)
        {
            delay1->go();
            continue;
        }

        if (delay1->recvPacket.front()->time() <= delay2->recvPacket.front()->time())
            delay1->go();
        else
            delay2->go();
    }

    int a = 0;
/*
    ad1->setNextProcessor(proc1->getNextProcessor());
    ad2->setNextProcessor(proc2->getNextProcessor());
    proc1->getNextProcessor()->setPrevProcessor(ad1);
    proc2->getNextProcessor()->setPrevProcessor(ad1);

    proc1->setSelf(IProcessorPtr());
    proc2->setSelf(IProcessorPtr());

    proc1->setNextProcessor(IProcessorPtr());
    proc2->setNextProcessor(IProcessorPtr());
    proc1->setPrevProcessor(IProcessorPtr());
    proc2->setPrevProcessor(IProcessorPtr());
    */
}