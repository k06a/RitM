#ifndef ICMPPROCESSOR_H
#define ICMPPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractPacketProcessor.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class IcmpProcessor : public AbstractProcessor
    {
    public:
        IcmpProcessor(IPacketProcessor * router = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class IcmpProcessor
}
// namespace DiplomBukov

#endif // ICMPPROCESSOR_H