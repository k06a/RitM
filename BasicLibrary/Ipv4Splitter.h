#ifndef IPV4SPLITTERPROCESSOR_H
#define IPV4SPLITTERPROCESSOR_H

#include <map>
#include <utility>

#include "CommonInclude.h"
#include "IPacket.h"
#include "AbstractProcessor.h"
#include "network/ipv4_header.h"

namespace DiplomBukov
{
    class Ipv4Splitter : public AbstractProcessor
    {
        typedef std::pair<ipv4_addr,ipv4_addr> ipv4_pair;
        typedef std::map<ipv4_pair,ProcessorPtr> MyMap;

        MyMap Connectors;
        ipv4_pair para;
        
    public:
        Ipv4Splitter(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;
        virtual void DestroyHierarchy();

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class Ipv4Splitter

    typedef SharedPointer<Ipv4Splitter>::Type Ipv4SplitterPtr;
}
// namespace DiplomBukov

#endif // IPV4SPLITTERPROCESSOR_H