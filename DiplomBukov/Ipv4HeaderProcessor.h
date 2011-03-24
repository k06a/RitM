#ifndef IPV4HEADERPROCESSOR_H
#define IPV4HEADERPROCESSOR_H

#include <map>
#include <utility>

#include "CommonInclude.h"
#include "IPacket.h"
#include "AbstractProcessor.h"
#include "network/ipv4_header.h"

namespace DiplomBukov
{
    class Ipv4HeaderProcessor : public AbstractProcessor
    {
        ipv4_header header;
        
    public:
        Ipv4HeaderProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class Ipv4HeaderProcessor
}
// namespace DiplomBukov

#endif // IPV4HEADERPROCESSOR_H