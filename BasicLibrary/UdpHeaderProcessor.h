#ifndef UDPHEADERPROCESSOR_H
#define UDPHEADERPROCESSOR_H

#include <vector>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "network/udp_header.h"

namespace RitM
{
    class UdpHeaderProcessor : public AbstractProcessor
    {
        Protocol inproto;
        udp_header header;

    public:
        UdpHeaderProcessor(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class UdpHeaderProcessor

    typedef SharedPointer<UdpHeaderProcessor>::Type UdpHeaderProcessorPtr;
}
// namespace RitM

#endif // UDPHEADERPROCESSOR_H