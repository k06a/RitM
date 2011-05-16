#ifndef MACSWITCHPORT_H
#define MACSWITCHPORT_H

#include <deque>
#include <algorithm>

#include "CommonInclude.h"
#include "IProcessor.h"
#include "AbstractProcessor.h"
#include "network/mac_header.h"

namespace RitM
{
    class MacSwitchPort;
    typedef std::tr1::shared_ptr<MacSwitchPort> MacSwitchPortPtr;

    class MacSwitchPort : public AbstractProcessor
    {
        typedef std::deque<mac_addr> MyList;

        MyList macList;

    public:
        MacSwitchPort(ProcessorPtr nextProcessor);
        MacSwitchPort(const MacSwitchPort & macSwitchPort);

        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual const char * getProcessorName();

        bool checkMac(const mac_addr & mac);
        //bool operator == (const mac_addr & mac);
    };
    // class MacSwitchPort
}
// namespace RitM

#endif // MACSWITCHPORT_H