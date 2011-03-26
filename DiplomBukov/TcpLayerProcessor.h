#ifndef TCPLAYERPROCESSOR_H
#define TCPLAYERPROCESSOR_H

#include <deque>
#include "i64u64.h"
#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "network/tcp_header.h"

namespace DiplomBukov
{
    class TcpLayerProcessor : public AbstractProcessor
    {
        enum ConnectionStatus
        {
            CLOSED,        // No any activity
            FIRST_SYN,     // Client sends SYN and waits
            SECOND_SYNACK, // Server sends SYN + ACK
            SECOND_ACK,    // Client sends ACK
            ESTABLISHED,   // Connection established
            PRELAST_FIN,   // One send FIN
            LAST_FINACK,   // Second send FIN + ACK
            LAST_ACK,      // Second send ACK
        };

        struct QuededPacket
        {
            u32be sequnceNumber;
            Protocol proto;
            PacketPtr packet;
            unsigned offset;
            unsigned dataInTcp;

            QuededPacket(u32be sequnceNumber = 0, Protocol proto = Protocol(),
                PacketPtr packet = PacketPtr(), unsigned offset = 0,
                unsigned dataInTcp = 0)
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

            bool operator == (const u32be & sn) const
            {
                return (sequnceNumber == sn);
            }
        };

        struct Abonent
        {
            u32be initialSN;
            u32be currentSendSN;
            u32be currentRecvSN;
            std::deque<QuededPacket> commitWaitQueue; // Отправленный пакет, ожидающий подтверждения
            std::deque<QuededPacket> recvWaitQueue;   // Очередь принятых пакетов ожидающих завершения
            std::deque<QuededPacket> toSendBuffer;    // Очередь пакетов на отправку

            QuededPacket lastAck;
            ConnectionStatus status;
            bool shouldCloseConnection;

            Abonent(u32be initialSN = 0,
                    u32be currentSendSN = 0,
                    u32be currentRecvSN = 0)
                : initialSN(initialSN)
                , currentSendSN(currentSendSN)
                , currentRecvSN(currentRecvSN)
                , commitWaitQueue()
                , recvWaitQueue()
                , lastAck()
                , status(CLOSED)
                , shouldCloseConnection(false)
            {
            }
        };

        Abonent client;
        Abonent server;
        unsigned maxDataInTcp;

    public:
        TcpLayerProcessor(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;
        virtual void DestroyHierarchy();

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

    private:
        virtual ProcessingStatus privateBackwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        PacketPtr createAck(const QuededPacket & qpacket);
        std::pair<PacketPtr,unsigned> mergePackets(const std::deque<QuededPacket> & arr);
    };
    // class TcpLayerProcessor
}
// namespace DiplomBukov

#endif // TCPLAYERPROCESSOR_H