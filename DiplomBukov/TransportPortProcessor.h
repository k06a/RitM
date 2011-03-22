#ifndef TCPPORTPROCESSOR_H
#define TCPPORTPROCESSOR_H

#include <map>
#include "IPacket.h"
#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "network/tcp_header.h"

namespace DiplomBukov
{
    class TransportPortProcessor : public AbstractProcessor
    {
        Protocol inProt;
        u16be server_port;
        u16be client_port;
        
    public:
        TransportPortProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TransportPortProcessor
}
// namespace DiplomBukov

#endif // TCPPORTPROCESSOR_H