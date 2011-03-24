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
        RejectProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class RejectProcessor
}
// namespace DiplomBukov

#endif // REJECTPROCESSOR_H