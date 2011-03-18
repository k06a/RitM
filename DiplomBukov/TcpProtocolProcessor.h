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
    class TcpProtocolProcessor : public AbstractProcessor
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
                    : sequnceNumber(sequnceNumber), proto(proto)
                    , packet(packet), offset(offset), dataInTcp(dataInTcp)
                {
                }

                QuededPacket()
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

                bool operator == (const u32be & sn) const
                {
                    return (sequnceNumber == sn);
                }
            };

            u32be initialSN;
            u32be currentSendSN;
            u32be currentRecvSN;
            std::deque<QuededPacket> sendWaitQueue; // Очередь отправленных пакетов ожидающих подтверждения
            std::deque<QuededPacket> recvWaitQueue; // Очередь принятых пакетов ожидающих завершения
            std::deque<QuededPacket> toSendBuffer;  // Очередь пакетов на отправку

            QuededPacket lastAck;
        };

        AbonentSN client;
        AbonentSN server;
        
    public:
        TcpProtocolProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

    private:
        IPacketPtr createAck(u32be seq, u32be ack, IPacketPtr packet, int dataInTcp, unsigned offset, AbonentSN & abonent);
        IPacketPtr createAck(AbonentSN::QuededPacket * qpacket);
        std::pair<IPacketPtr,unsigned> mergePackets(const std::deque<AbonentSN::QuededPacket> & arr);
    };
    // class TcpProtocolProcessor
}
// namespace DiplomBukov

#endif // TCPSEQUENCEPROCESSOR_H