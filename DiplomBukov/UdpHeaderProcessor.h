#ifndef UDPHEADERPROCESSOR_H
#define UDPHEADERPROCESSOR_H

#include <vector>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "network/udp_header.h"

namespace DiplomBukov
{
    class UdpHeaderProcessor : public AbstractProcessor
    {
        Protocol inproto;
        udp_header header;

    public:
        UdpHeaderProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class UdpHeaderProcessor
}
// namespace DiplomBukov

#endif // UDPHEADERPROCESSOR_H