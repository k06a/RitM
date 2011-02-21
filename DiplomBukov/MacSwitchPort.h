#ifndef MACSWITCHPORT_H
#define MACSWITCHPORT_H

#include <deque>
#include <algorithm>
#include "CommonInclude.h"
#include "IPacketProcessor.h"
#include "mac_header.h"

namespace DiplomBukov
{
    class MacSwitchPort : public IPacketProcessor
    {
        typedef std::deque<mac_addr> MyList;

        IPacketProcessor * nextProcessor;
        IPacketProcessor * prevProcessor;
        MyList macList;

    public:
        MacSwitchPort(IPacketProcessor * nextProcessor);
        MacSwitchPort(const MacSwitchPort & macSwitchPort);
        void Init(const IPacketProcessor * np, const IPacketProcessor * pp);
        virtual IPacketProcessor * CreateCopy() const;

        virtual IPacketProcessor * getPointer();
        virtual void ping(IPacketProcessor * prevProcessor);
        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset);

        bool checkMac(const mac_addr & mac);
    };
    // class MacSwitchPort
}
// namespace DiplomBukov

#endif // MACSWITCHPORT_H