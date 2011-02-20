#ifndef ISWITCHPORT_H
#define ISWITCHPORT_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class ISwitchPort : public IPacketProcessor
    {
    public:
        virtual ~ISwitchPort() {}

        virtual ISwitchPort * CreateCopy() const = 0;

        virtual void setSwitch(ISwitch * switch) = 0;
        virtual ISwitch * getSwitch() = 0;
    };
    // class ISwitchPort
}
// namespace DiplomBukov

#endif // ISWITCHPORT_H