#ifndef ICMPPROCESSOR_H
#define ICMPPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class IcmpProcessor : public AbstractProcessor
    {
    public:
        IcmpProcessor(IProcessorPtr router = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class IcmpProcessor
}
// namespace DiplomBukov

#endif // ICMPPROCESSOR_H