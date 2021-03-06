#ifndef TCPHEADERPROCESSOR_H
#define TCPHEADERPROCESSOR_H

#include <vector>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "network/tcp_header.h"

namespace RitM
{
    class TcpHeaderProcessor : public AbstractProcessor
    {
        Protocol inproto;
        std::vector<u8> header;

    public:
        TcpHeaderProcessor(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpHeaderProcessor

    typedef SharedPointer<TcpHeaderProcessor>::Type TcpHeaderProcessorPtr;
}
// namespace RitM

#endif // TCPHEADERPROCESSOR_H