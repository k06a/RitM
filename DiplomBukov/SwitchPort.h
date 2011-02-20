#ifndef SWITCHPORT_H
#define SWITCHPORT_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class SwitchPort : public IPacketProcessor
    {        
    public:
        virtual ISwitchPort * CreateCopy() const;
        virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);

        void setSwitch(ISwitch * switch);
        ISwitch * getSwitch();
    };
    // class SwitchPort
}
// namespace DiplomBukov

#endif // SWITCHPORT_H