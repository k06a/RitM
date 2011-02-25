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
        typedef std::multimap<Protocol,IProcessorPtr> MyMap;

        MyMap procMap;

    public:
        ProtocolRouter();
        ProtocolRouter(const ProtocolRouter & router);
        void Init(const MyDeque & d);
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual void addNextProcessor(IProcessorPtr processor);
        virtual void removeNextProcessor(IProcessorPtr processor);
        virtual const std::deque<IProcessorPtr> & nextProcessors();
    };
    // class ProtocolRouter
}
// namespace DiplomBukov

#endif // PROTOCOLROUTER_H