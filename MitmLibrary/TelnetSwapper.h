#ifndef TELNETSWAPPER_H
#define TELNETSWAPPER_H

#include <deque>

#include "CommonInclude.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class TelnetSwapper : public AbstractProcessor
    {
    public:
        TelnetSwapper(ProcessorPtr processor = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TelnetSwapper

    typedef SharedPointer<TelnetSwapper>::Type TelnetSwapperPtr;
}
// namespace DiplomBukov

#endif // TELNETSWAPPER_H