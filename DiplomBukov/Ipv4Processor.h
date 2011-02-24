#ifndef IPV4PROCESSOR_H
#define IPV4PROCESSOR_H

#include <map>
#include <utility>

#include "CommonInclude.h"
#include "IPacket.h"
#include "AbstractProcessor.h"
#include "ipv4_header.h"

namespace DiplomBukov
{
    class Ipv4Processor : public AbstractProcessor
    {
    public:
        Ipv4Processor(IProcessorPtr router = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class Ipv4Processor
}
// namespace DiplomBukov

#endif // IPV4PROCESSOR_H