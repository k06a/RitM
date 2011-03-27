#ifndef MACSWITCH_H
#define MACSWITCH_H

#include <deque>
#include "CommonInclude.h"
#include "IPacket.h"
#include "IProcessor.h"
#include "AbstractProcessor.h"
#include "ProtocolConnector.h"
#include "MacSwitchPort.h"

namespace DiplomBukov
{
    class MacSwitch;
    typedef SmartPointer<MacSwitch>::Type MacSwitchPtr;

    class MacSwitch : public AbstractProcessor
    {
        typedef std::deque<MacSwitchPortPtr> MyDeque;

        MyDeque ports;

    public:
        MacSwitch(ProcessorPtr Connector = ProcessorPtr());
        MacSwitch(const MacSwitch & macSwitch);
        void DestroyHierarchy();

        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessorPtr getPointer();
        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual const char * MacSwitch::getProcessorName();
    };
    // class MacSwitch
}
// namespace DiplomBukov

#endif // MACSWITCH_H