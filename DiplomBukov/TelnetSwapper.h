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
        TelnetSwapper(IProcessorPtr processor = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TelnetSwapper
}
// namespace DiplomBukov

#endif // TELNETSWAPPER_H