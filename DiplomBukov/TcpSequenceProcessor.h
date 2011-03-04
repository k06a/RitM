#ifndef TCPSEQUENCEPROCESSOR_H
#define TCPSEQUENCEPROCESSOR_H

#include <map>
#include "IPacket.h"
#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "tcp_header.h"
#include "i64u64.h"

namespace DiplomBukov
{
    class TcpSequenceProcessor : public AbstractProcessor
    {
        u32be originClientSN;
        u32be originServerSN;
        u32be actualClientSN;
        u32be actualServerSN;

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