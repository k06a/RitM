#ifndef PROTOCOLCONNECTOR_H
#define PROTOCOLCONNECTOR_H

#include <map>
#include <deque>

#include "CommonInclude.h"
#include "AbstractConnector.h"

namespace RitM
{
    class ProtocolConnector : public AbstractConnector
    {
        typedef std::multimap<Protocol,ProcessorPtr> MyMap;

        MyMap procMap;

    public:
        ProtocolConnector();
        ProtocolConnector(const MyDeque & d);
        virtual ProcessorPtr CreateCopy() const;

        virtual void ping(ProcessorPtr prevProcessor);

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual void addNextProcessor(ProcessorPtr processor);
        virtual void removeNextProcessor(ProcessorPtr processor);
        virtual const std::deque<ProcessorPtr> & nextProcessors();
    };
    // class ProtocolConnector

    typedef SharedPointer<ProtocolConnector>::Type ProtocolConnectorPtr;
}
// namespace RitM

#endif // PROTOCOLCONNECTOR_H