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
        ProtocolRouter(const MyDeque & d, const MyMap m);
        
        virtual IRouter * CreateCopy() const;
        virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);

        virtual void addNextProcessor(IProcessor * processor);
        virtual void removeNextProcessor(IProcessor * processor);
        virtual const std::deque<IProcessor*> & nextProcessors();
    };
    // class ProtocolRouter
}
// namespace DiplomBukov

#endif // PROTOCOLROUTER_H