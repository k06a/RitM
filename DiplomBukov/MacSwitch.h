#ifndef MACSWITCH_H
#define MACSWITCH_H

#include <deque>
#include "CommonInclude.h"
#include "IProcessor.h"
#include "AbstractProcessor.h"
#include "ProtocolRouter.h"

namespace DiplomBukov
{
    class MacSwitchPort;

    class MacSwitch : public AbstractProcessor
    {
        std::deque<MacSwitchPort*> ports;

    public:
        MacSwitch(IProcessorPtr router = IProcessorPtr());
        MacSwitch(const MacSwitch & macSwitch);

        virtual IProcessorPtr CreateCopy() const;

        virtual IProcessorPtr getPointer();
        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
    };
    // class MacSwitch
}
// namespace DiplomBukov

#endif // MACSWITCH_H