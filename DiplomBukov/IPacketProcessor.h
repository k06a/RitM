#ifndef IPACKETPROCESSOR_H
#define IPACKETPROCESSOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IPacketProcessor
    {
    public:
        virtual ~IPacketProcessor() {}

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset) = 0;
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset) = 0;

        virtual IPacketProcessor * CreateCopy() const = 0;
        virtual IPacketProcessor * getPointer() = 0;
        virtual void ping(IPacketProcessor * prevProcessor) = 0;

        virtual void setNextProcessor(IPacketProcessor * processor) = 0;
        virtual IPacketProcessor * getNextProcessor() = 0;
        virtual void setPrevProcessor(IPacketProcessor * processor) = 0;
        virtual IPacketProcessor * getPrevProcessor() = 0;
    };
    // class IPacketProcessor
}
// namespace DiplomBukov

#endif // IPACKETPROCESSOR_H