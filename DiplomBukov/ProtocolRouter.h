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
        std::deque<IProcessor*> nextProcessors;
        std::multimap<Protocol,IProcessor*> mapProcessors;

    public:
        virtual void transmitPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual void addNextProcessor(IProcessor * processor);
        virtual void removeNextProcessor(IProcessor * processor);
        virtual const std::deque<IProcessor*> & processors();
    };
    // class ProtocolRouter
}
// namespace DiplomBukov

#endif // PROTOCOLROUTER_H