#ifndef TCPHEADERPROCESSOR_H
#define TCPHEADERPROCESSOR_H

#include <map>
#include "IPacket.h"
#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "tcp_header.h"

namespace DiplomBukov
{
    class TcpHeaderProcessor : public AbstractProcessor
    {
        tcp_header header;
        u16 & server_port;
        u16 & client_port;
        u32 & serverSequenceNumber;
        u32 & clientSequenceNumber;

    public:
        TcpHeaderProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpHeaderProcessor
}
// namespace DiplomBukov

#endif // TCPHEADERPROCESSOR_H