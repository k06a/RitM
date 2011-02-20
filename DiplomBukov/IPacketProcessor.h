#ifndef IPACKETPROCESSOR_H
#define IPACKETPROCESSOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IPacketProcessor
    {
    public:
        virtual ~IPacketProcessor() {}

        virtual IPacketProcessor * CreateCopy() const = 0;
        virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset) = 0;
    };
    // class IPacketProcessor
}
// namespace DiplomBukov

#endif // IPACKETPROCESSOR_H