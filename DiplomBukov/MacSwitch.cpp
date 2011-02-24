#include "MacSwitch.h"
#include "MacSwitchPort.h"

using namespace DiplomBukov;

MacSwitch::MacSwitch(IProcessorPtr router)
{
    nextProcessor = router;
}

MacSwitch::MacSwitch(const MacSwitch & macSwitch)
{
    for(MyDeque::const_iterator it = macSwitch.ports.begin();
        it != macSwitch.ports.end(); ++it)
    {
        ports.push_back(MacSwitchPortPtr((*it)->CreateCopy()));
    }
}

IProcessorPtr MacSwitch::CreateCopy() const
{
    return IProcessorPtr(new MacSwitch(*this));
}

IProcessorPtr MacSwitch::getPointer()
{
    //TODO: fixes
    MacSwitchPort * port = new MacSwitchPort(Self);
    ports.push_back(MacSwitchPortPtr(port));
    return IProcessorPtr(port);
}

ProcessingStatus MacSwitch::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    return nextProcessor->forwardProcess(proto, packet, offset);
}

ProcessingStatus MacSwitch::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    //
    return ProcessingStatus::Accepted;
}
