#ifndef PROTOCOLCONNECTOR_H
#define PROTOCOLCONNECTOR_H

#include <map>
#include <deque>

#include "CommonInclude.h"
#include "AbstractConnector.h"

namespace DiplomBukov
{
    class ProtocolConnector : public AbstractConnector
    {
        typedef std::multimap<Protocol,IProcessorPtr> MyMap;

        MyMap procMap;

    public:
        ProtocolConnector();
        ProtocolConnector(const ProtocolConnector & Connector);
        void Init(const MyDeque & d);
        virtual IProcessorPtr CreateCopy() const;

        virtual void ping(IProcessorPtr prevProcessor);

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual void addNextProcessor(IProcessorPtr processor);
        virtual void removeNextProcessor(IProcessorPtr processor);
        virtual const std::deque<IProcessorPtr> & nextProcessors();
    };
    // class ProtocolConnector
}
// namespace DiplomBukov

#endif // PROTOCOLCONNECTOR_H