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
        //                   <Seq>
        //   |--------|--------V--------|--------|
        // 1.    |--------|                         // OldPacket
        // 2.                  |                    // ExpectedPacket
        // 4.            |--------|                 // CutPacket
        // 8.                      |--------|       // FuturePacket
        //

        enum PacketSituation
        {
            OldPacket      = 1, // Пакет из прошлого
            ExpectedPacket = 2, // Пакет с ожидаемым номером
            CutPacket      = 4, // Пакет с частью новых данных
            FuturePacket   = 8, // Пакет из будущего
            CommitPacket   = 16 // Пакет с подтверждением
        };

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
                    : sequnceNumber(sequnceNumber), proto(proto)
                    , packet(packet), offset(offset), dataInTcp(dataInTcp)
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
            std::deque<QuededPacket> sendWaitQueue;
            std::deque<QuededPacket> recvWaitQueue;
            std::deque<QuededPacket> toSendBuffer;
            std::deque<QuededPacket> toRecvBuffer;
        };

        AbonentSN client;
        AbonentSN server;
        
    public:
        TcpSequenceProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus forwardProcess2(AbonentSN::QuededPacket * packet, PacketSituation situation);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

    private:
        IPacketPtr createAck(AbonentSN::QuededPacket * qpacket, AbonentSN & abonent);
        std::pair<IPacketPtr,unsigned> mergePackets(const std::deque<AbonentSN::QuededPacket> & arr);
    };
    // class TcpSequenceProcessor
}
// namespace DiplomBukov

#endif // TCPSEQUENCEPROCESSOR_H