#include "MacSwitchPort.h"

using namespace DiplomBukov;

MacSwitchPort::MacSwitchPort(IPacketProcessor * const nextProcessor)
{
    this->nextProcessor = nextProcessor;
    this->prevProcessor = NULL;
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

IProcessor * MacSwitchPort::CreateCopy() const
{
    return new MacSwitchPort(*this);
}

ProcessingStatus MacSwitchPort::forwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    return ProcessingStatus::Accepted;
}

ProcessingStatus MacSwitchPort::backwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    return ProcessingStatus::Accepted;
}

const char * MacSwitchPort::getProcessorName()
{
    return "MacSwitchPort";
}

bool MacSwitchPort::checkMac(const mac_addr & mac)
{
    MyList::iterator it = std::find(macList.begin(), macList.end(), mac);
    return (it != macList.end());
}
