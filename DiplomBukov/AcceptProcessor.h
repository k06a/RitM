#ifndef ACCEPTPROCESSOR_H
#define ACCEPTPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"

namespace DiplomBukov
{
    class AcceptProcessor : public AbstractProcessor
    {
    public:
        AcceptProcessor(IProcessorPtr router = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class AcceptProcessor
}
// namespace DiplomBukov

#endif // ACCEPTPROCESSOR_H