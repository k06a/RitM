#ifndef ICMPPROCESSOR_H
#define ICMPPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"

namespace DiplomBukov
{
    class IcmpHeaderProcessor : public AbstractProcessor
    {
    public:
        IcmpHeaderProcessor(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class IcmpHeaderProcessor

    typedef SharedPointer<IcmpHeaderProcessor>::Type IcmpProcessorPtr;
}
// namespace DiplomBukov

#endif // ICMPPROCESSOR_H