#ifndef ICMPPROCESSOR_H
#define ICMPPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"

namespace DiplomBukov
{
    class IcmpProcessor : public AbstractProcessor
    {
    public:
        IcmpProcessor(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class IcmpProcessor
}
// namespace DiplomBukov

#endif // ICMPPROCESSOR_H