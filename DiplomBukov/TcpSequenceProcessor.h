#ifndef TCPSEQUENCEPROCESSOR_H
#define TCPSEQUENCEPROCESSOR_H

#include <deque>
#include "IPacket.h"
#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "tcp_header.h"
#include "triplet.h"
#include "i64u64.h"

namespace DiplomBukov
{
    class TcpSequenceProcessor : public AbstractProcessor
    {
        struct AbonentSN
        {
            struct QuededPacket
            {
                u32be sequnceNumber;
                Protocol proto;
                IPacketPtr packet;
                unsigned offset;
                unsigned dataInTcp;

                QuededPacket(u32be sequnceNumber, Protocol proto,
                             IPacketPtr packet, unsigned offset,
                             unsigned dataInTcp)
                    : sequnceNumber(sequnceNumber)
                    , proto(proto)
                    , packet(packet)
                    , offset(offset)
                    , dataInTcp(dataInTcp)
                {
                }

                bool operator < (const QuededPacket & wp) const
                {
                    return (sequnceNumber < wp.sequnceNumber);
                }

                bool operator == (const QuededPacket & wp) const
                {
                    return (sequnceNumber == wp.sequnceNumber);
                }
            };

            u32be initialSN;
            u32be currentSendSN;
            u32be currentRecvSN;
            std::deque<QuededPacket> sendBuffer;
            std::deque<QuededPacket> recvBuffer;
            // We need triplet for sorting by seq using std::inplace_merge()
        };

        AbonentSN client;
        AbonentSN server;
        
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