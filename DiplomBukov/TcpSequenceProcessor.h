#ifndef TCPSEQUENCEPROCESSOR_H
#define TCPSEQUENCEPROCESSOR_H

#include <deque>
#include "i64u64.h"
#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "tcp_header.h"

namespace DiplomBukov
{
    class TcpSequenceProcessor : public AbstractProcessor
    {
        struct Abonent
        {
            u32be initialSN;
            u32be currentSendSN;
            u32be currentRecvSN;
        };

        Abonent client;
        Abonent server;

    public:
        TcpSequenceProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpSequenceProcessor
}
// namespace DiplomBukov

#endif // TCPSEQUENCEPROCESSOR_H