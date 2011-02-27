#ifndef MACSWITCH_H
#define MACSWITCH_H

#include <deque>
#include "CommonInclude.h"
#include "IProcessor.h"
#include "AbstractProcessor.h"
#include "ProtocolConnector.h"
#include "MacSwitchPort.h"

namespace DiplomBukov
{
    class MacSwitchPort;

    class MacSwitch : public AbstractProcessor
    {
        typedef std::deque<IProcessorPtr> MyDeque;

        MyDeque ports;

    public:
        MacSwitch(IProcessorPtr Connector = IProcessorPtr());
        MacSwitch(const MacSwitch & macSwitch);

        virtual IProcessorPtr CreateCopy() const;

        virtual IProcessorPtr getPointer();
        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
    };
    // class MacSwitch
}
// namespace DiplomBukov

#endif // MACSWITCH_H