#include "MacSwitch.h"
#include "MacSwitchPort.h"

using namespace DiplomBukov;

MacSwitch::MacSwitch(IPacketProcessor * router)
{
    nextProcessor = router;
}

MacSwitch::MacSwitch(const MacSwitch & macSwitch)
{
}

IProcessor * MacSwitch::CreateCopy() const
{
    return new MacSwitch(*this);
}

IPacketProcessor * MacSwitch::getPointer()
{
    MacSwitchPort * port = new MacSwitchPort(this);
    ports.push_back(port);
    return port;
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
