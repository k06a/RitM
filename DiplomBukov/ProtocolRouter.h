#ifndef PROTOCOLROUTER_H
#define PROTOCOLROUTER_H

#include <map>
#include <deque>

#include "CommonInclude.h"
#include "AbstractRouter.h"

namespace DiplomBukov
{
    class ProtocolRouter : public AbstractRouter
    {
        typedef std::multimap<Protocol,IProcessor*> MyMap;

        MyMap procMap;

    public:
        ProtocolRouter();
        ProtocolRouter(const ProtocolRouter & router);
        void Init(const MyDeque & d);
        virtual IRouter * CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        
        virtual void addNextProcessor(IProcessor * processor);
        virtual void removeNextProcessor(IProcessor * processor);
        virtual const std::deque<IProcessor*> & nextProcessors();
    };
    // class ProtocolRouter
}
// namespace DiplomBukov

#endif // PROTOCOLROUTER_H