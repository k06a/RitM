#ifndef REJECTPROCESSOR_H
#define REJECTPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"

namespace DiplomBukov
{
    class RejectProcessor : public AbstractProcessor
    {
    public:
        RejectProcessor(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class RejectProcessor

    typedef SharedPointer<RejectProcessor>::Type RejectProcessorPtr;
}
// namespace DiplomBukov

#endif // REJECTPROCESSOR_H