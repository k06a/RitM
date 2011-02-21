#ifndef PROTOCOLROUTER_H
#define PROTOCOLROUTER_H

#include <map>
#include <deque>
#include "IRouter.h"
#include "IProcessor.h"

namespace DiplomBukov
{
    class ProtocolRouter : public IRouter
    {
        typedef std::deque<IProcessor*> MyDeque;
        typedef std::multimap<Protocol,IProcessor*> MyMap;

        MyDeque procList;
        MyMap procMap;

    public:
        ProtocolRouter();
        ProtocolRouter(const ProtocolRouter & router);
        void Init(const MyDeque & d, const MyMap & m);
        virtual IRouter * CreateCopy() const;

        virtual IPacketProcessor * getPointer();
        virtual void ping(IPacketProcessor * prevProcessor);
        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual void addNextProcessor(IProcessor * processor);
        virtual void removeNextProcessor(IProcessor * processor);
        virtual const std::deque<IProcessor*> & nextProcessors();
    };
    // class ProtocolRouter
}
// namespace DiplomBukov

#endif // PROTOCOLROUTER_H