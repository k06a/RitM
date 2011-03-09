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
            CLOSED,        // No any activity
            FIRST_SYN,     // Client sends SYN and waits
            FIRST_ACK,     // Server answered ACK
            SECOND_SYN,    // Server sends SYN
            SECOND_ACK,    // Server sends SYN
            ESTABLISHED,   // Connection established
            PRELAST_FIN,   // One send FIN and waits for 2*MSL secs
            PRELAST_ACK,   // Second send 
            LAST_FIN,      // Second send 
            LAST_ACK,      // Second send 
        };

        ConnectionStatus connectionStatus;

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