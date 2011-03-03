#ifndef UDPHEADERPROCESSOR_H
#define UDPHEADERPROCESSOR_H

#include "TcpHeaderProcessor.h"
#include "udp_header.h"

namespace DiplomBukov
{
    class UdpHeaderProcessor : public TcpHeaderProcessor
    {
        udp_header header;
        u16be & server_port;
        u16be & client_port;

    public:
        UdpHeaderProcessor(IProcessorPtr Connector = IProcessorPtr());

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class UdpHeaderProcessor
}
// namespace DiplomBukov

#endif // UDPHEADERPROCESSOR_H