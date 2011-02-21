#ifndef MACSWITCHPORT_H
#define MACSWITCHPORT_H

#include <deque>
#include <algorithm>

#include "CommonInclude.h"
#include "IPacketProcessor.h"
#include "AbstractProcessor.h"
#include "mac_header.h"

namespace DiplomBukov
{
    class MacSwitchPort : public AbstractProcessor
    {
        typedef std::deque<mac_addr> MyList;

        MyList macList;

    public:
        MacSwitchPort(IPacketProcessor * const nextProcessor);
        MacSwitchPort(const MacSwitchPort & macSwitchPort);
        void Init(const IPacketProcessor * np, const IPacketProcessor * pp);
        virtual IProcessor * CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset);
        virtual const char * getProcessorName();

        bool checkMac(const mac_addr & mac);
    };
    // class MacSwitchPort
}
// namespace DiplomBukov

#endif // MACSWITCHPORT_H