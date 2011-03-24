#ifndef TRANSPORTPORTFILTER_H
#define TRANSPORTPORTFILTER_H

#include <map>
#include "IPacket.h"
#include "CommonInclude.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class TransportPortFilter : public AbstractProcessor
    {
        IOptionPtr portOption;
        std::deque<IOptionPtr> options;

    public:
        TransportPortFilter(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        virtual std::deque<IOptionPtr> getOptions();
    };
    // class TransportPortFilter
}
// namespace DiplomBukov

#endif // TRANSPORTPORTFILTER_H