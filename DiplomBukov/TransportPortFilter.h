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
        OptionPtr portOption;

    public:
        TransportPortFilter(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        virtual OptionPtr getOptions();
    };
    // class TransportPortFilter
}
// namespace DiplomBukov

#endif // TRANSPORTPORTFILTER_H