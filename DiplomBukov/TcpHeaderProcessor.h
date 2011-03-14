#ifndef TCPHEADERPROCESSOR_H
#define TCPHEADERPROCESSOR_H

#include <map>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "tcp_header.h"

namespace DiplomBukov
{
    class TcpHeaderProcessor : public AbstractProcessor
    {
        Protocol inproto;

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