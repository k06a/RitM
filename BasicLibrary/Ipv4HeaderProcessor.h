#ifndef IPV4HEADERPROCESSOR_H
#define IPV4HEADERPROCESSOR_H

#include <map>
#include <utility>

#include "CommonInclude.h"
#include "IPacket.h"
#include "AbstractProcessor.h"
#include "network/ipv4_header.h"

namespace RitM
{
    class Ipv4HeaderProcessor : public AbstractProcessor
    {
        std::vector<u8> header;
        
    public:
        Ipv4HeaderProcessor(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class Ipv4HeaderProcessor

    typedef SharedPointer<Ipv4HeaderProcessor>::Type Ipv4HeaderProcessorPtr;
}
// namespace RitM

#endif // IPV4HEADERPROCESSOR_H