#ifndef MACSWITCH_H
#define MACSWITCH_H

#include "CommonInclude.h"
#include "ISwitch.h"

using namespace DiplomBukov
{
    class MacSwitch : public ISwitch
    {
        public:
        virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset, );
    };
    // class MacSwitch
}
// using namespace DiplomBukov

#endif // MACSWITCH_H