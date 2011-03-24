#ifndef MACSWITCHPORT_H
#define MACSWITCHPORT_H

#include <deque>
#include <algorithm>

#include "CommonInclude.h"
#include "IProcessor.h"
#include "AbstractProcessor.h"
#include "network/mac_header.h"

namespace DiplomBukov
{
    class MacSwitchPort : public AbstractProcessor
    {
        typedef std::deque<mac_addr> MyList;

        MyList macList;

    public:
        MacSwitchPort(IProcessorPtr const nextProcessor);
        MacSwitchPort(const MacSwitchPort & macSwitchPort);

        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);
        virtual const char * getProcessorName();

        bool checkMac(const mac_addr & mac);
        //bool operator == (const mac_addr & mac);
    };
    // class MacSwitchPort

    typedef std::tr1::shared_ptr<MacSwitchPort> MacSwitchPortPtr;
}
// namespace DiplomBukov

#endif // MACSWITCHPORT_H