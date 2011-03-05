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
            CLOSED,      // No any activity
            SYN_FIRST,   // Client sends SYN and waits
            SYN_SECOND,  // Server answered ACK, and send SYN and waits
            ESTABLISHED, // Connection established
            TIME_WAIT    // Person received FIN and waits for 2*MSL secs
        };

        ConnectionStatus connectionStatus;
        tcp_header::flags_struct flagsForClient;
        tcp_header::flags_struct flagsForServer;

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