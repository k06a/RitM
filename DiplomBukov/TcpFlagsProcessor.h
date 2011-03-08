#ifndef TCPFLAGSPROCESSOR_H
#define TCPFLAGSPROCESSOR_H

#include <map>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "tcp_header.h"

namespace DiplomBukov
{
    struct Abonent
    {
        static const int defaultWindowsSize = 65536;

        std::vector<u8> recvBuffer;
        std::vector<u8> sendBuffer;
        std::vector<u32be> waitAckNumbers;
        u32 recvBufferAckNumber;
        u32 sendBufferSeqNumber;
        u32be seqNumber;
        u32be ackNumber;
        u16be windowSize;

        Abonent()
            : recvBuffer(), sendBuffer()
            , recvBufferAckNumber(0)
            , sendBufferSeqNumber(0)
            , seqNumber(0), ackNumber(0)
        {
        }
    };

    class TcpFlagsProcessor : public AbstractProcessor
    {
        enum ConnectionStatus
        {
            CLOSED,      // No any activity
            SYN_FIRST,   // Client sends SYN and waits
            SYN_SECOND,  // Server answered ACK
            SYN_THIRD,   // Server sends SYN
            ESTABLISHED, // Connection established
            TIME_WAIT    // Person received FIN and waits for 2*MSL secs
        };

        ConnectionStatus connectionStatus;
        Abonent client;
        Abonent server;

    public:
        TcpFlagsProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        void transferDataToAbonentFromSN(IPacketPtr & packet, Abonent & abonent, u32be startSeqNumber);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpFlagsProcessor
}
// namespace DiplomBukov

#endif // TCPFLAGSPROCESSOR_H