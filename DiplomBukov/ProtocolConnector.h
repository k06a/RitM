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
        typedef std::multimap<Protocol,ProcessorPtr> MyMap;

        MyMap procMap;

    public:
        ProtocolConnector();
        ProtocolConnector(const ProtocolConnector & Connector);
        void Init(const MyDeque & d);
        virtual ProcessorPtr CreateCopy() const;

        virtual void ping(ProcessorPtr prevProcessor);

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual void addNextProcessor(ProcessorPtr processor);
        virtual void removeNextProcessor(ProcessorPtr processor);
        virtual const std::deque<ProcessorPtr> & nextProcessors();
    };
    // class ProtocolConnector
}
// namespace DiplomBukov

#endif // PROTOCOLCONNECTOR_H