#ifndef MACSWITCH_H
#define MACSWITCH_H

#include <deque>
#include "CommonInclude.h"
#include "IRouter.h"
#include "ProtocolRouter.h"

namespace DiplomBukov
{
    class MacSwitchPort;

    class MacSwitch : public IRouter
    {
        ProtocolRouter router;
        std::deque<MacSwitchPort*> ports;

    public:
        MacSwitch();
        MacSwitch(const MacSwitch & macSwitch);
        virtual IRouter * CreateCopy() const;

        virtual IPacketProcessor * getPointer();
        virtual void ping(IPacketProcessor * prevProcessor);
        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual void addNextProcessor(IProcessor * packetProcessor);
        virtual void removeNextProcessor(IProcessor * packetProcessor);
        virtual const std::deque<IProcessor*> & nextProcessors();
    };
    // class MacSwitch
}
// namespace DiplomBukov

#endif // MACSWITCH_H