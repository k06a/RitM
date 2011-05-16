#ifndef ICMPPROCESSOR_H
#define ICMPPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"

namespace RitM
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
// namespace RitM

#endif // ICMPPROCESSOR_H