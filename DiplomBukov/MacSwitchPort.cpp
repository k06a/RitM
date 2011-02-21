#include "MacSwitchPort.h"

using namespace DiplomBukov;

MacSwitchPort::MacSwitchPort(IPacketProcessor * nextProcessor)
    : nextProcessor(nextProcessor), prevProcessor(NULL)
{
}

MacSwitchPort::MacSwitchPort(const MacSwitchPort & macSwitchPort)
{
    Init(macSwitchPort.nextProcessor, macSwitchPort.prevProcessor);
}

void MacSwitchPort::Init(const IPacketProcessor * np, const IPacketProcessor * pp)
{
    nextProcessor = np->CreateCopy();
    prevProcessor = pp->CreateCopy();
}

IPacketProcessor * MacSwitchPort::CreateCopy() const
{
    return new MacSwitchPort(*this);
}

IPacketProcessor * MacSwitchPort::getPointer()
{
    return this;
}

void MacSwitchPort::ping(IPacketProcessor * prevProcessor)
{
    this->prevProcessor = prevProcessor;
}

ProcessingStatus MacSwitchPort::forwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    return ProcessingStatus::Accepted;
}

ProcessingStatus MacSwitchPort::backwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    return ProcessingStatus::Accepted;
}

bool MacSwitchPort::checkMac(const mac_addr & mac)
{
    MyList::iterator it = std::find(macList.begin(), macList.end(), mac);
    return (it != macList.end());
}
