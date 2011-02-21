#ifndef IPACKETPROCESSOR_H
#define IPACKETPROCESSOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IPacketProcessor
    {
        IPacketProcessor * prevProcessor;
        IPacketProcessor * nextProcessor;

    public:
        virtual ~IPacketProcessor() {}

        virtual IPacketProcessor * CreateCopy() const = 0;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset) = 0;
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset) = 0;

        virtual IPacketProcessor * getPointer() = 0;

        virtual void ping(IPacketProcessor * prevProcessor)
        {
            this->prevProcessor = prevProcessor;
        }

    };
    // class IPacketProcessor
}
// namespace DiplomBukov

#endif // IPACKETPROCESSOR_H