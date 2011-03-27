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
        AcceptProcessor(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class AcceptProcessor

    typedef SmartPointer<AcceptProcessor>::Type AcceptProcessorPtr;
}
// namespace DiplomBukov

#endif // ACCEPTPROCESSOR_H