#include "MacSwitch.h"
#include "MacSwitchPort.h"

using namespace DiplomBukov;

MacSwitch::MacSwitch(IProcessorPtr router)
{
    nextProcessor = router;
}

MacSwitch::MacSwitch(const MacSwitch & macSwitch)
{
}

IProcessorPtr MacSwitch::CreateCopy() const
{
    return IProcessorPtr(new MacSwitch(*this));
}

IProcessorPtr MacSwitch::getPointer()
{
    MacSwitchPort * port = new MacSwitchPort(IProcessorPtr(this));
    ports.push_back(port);
    return IProcessorPtr(port);
}

ProcessingStatus MacSwitch::forwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    return nextProcessor->forwardProcess(proto, packet, offset);
}

ProcessingStatus MacSwitch::backwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    //
    return ProcessingStatus::Accepted;
}
