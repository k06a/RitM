#ifndef TCPFLAGSPROCESSOR_H
#define TCPFLAGSPROCESSOR_H

#include <map>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "tcp_header.h"

namespace DiplomBukov
{
    class TcpFlagsProcessor : public AbstractProcessor
    {
        enum ConnectionStatus
        {
            CLOSED,         // Server wait for anyone connection
            LISTEN,         // Server waits for client requests
            SYN_SENT,       // Client sends SYN and waits
            SYN_RECEIVED,   // Server answered ACK, and send SYN and waits
            ESTABLISHED,    // Connection established
            FIN_WAIT_1,     // One side send FIN and waits
            CLOSE_WAIT,     // Other side answers ACK, ans sends data
            FIN_WAIT_2,     // Person witch sends first FIN receive data
            LAST_ACK,       // Other side sends FIN too
            TIME_WAIT,      // Person received FIN and waits for 2*MSL secs
            CLOSING         // First send FIN, and second send ACK+FIN and wait ACK
        };

        ConnectionStatus connectionStatus;
        tcp_header::flags_struct serverExpextedFlags;
        tcp_header::flags_struct clientExpextedFlags;

    public:
        TcpFlagsProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpFlagsProcessor
}
// namespace DiplomBukov

#endif // TCPFLAGSPROCESSOR_H