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
        MacSwitch(IPacketProcessor * router = NULL);
        MacSwitch(const MacSwitch & macSwitch);

        virtual IProcessor * CreateCopy() const;

        virtual IPacketProcessor * getPointer();
        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset);
    };
    // class MacSwitch
}
// namespace DiplomBukov

#endif // MACSWITCH_H