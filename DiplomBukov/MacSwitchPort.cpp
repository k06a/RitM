#include "MacSwitchPort.h"
#include "IPacket.h"

using namespace DiplomBukov;

MacSwitchPort::MacSwitchPort(IProcessorPtr const nextProcessor)
{
    this->nextProcessor = nextProcessor;
    this->prevProcessor = IProcessorPtr();
    macList.push_back(mac_addr::broadcast());
}

MacSwitchPort::MacSwitchPort(const MacSwitchPort & macSwitchPort)
{
    nextProcessor = macSwitchPort.nextProcessor;
    prevProcessor = IProcessorPtr();
}

IProcessorPtr MacSwitchPort::CreateCopy() const
{
    IProcessorPtr ptr(new MacSwitchPort(*this));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus MacSwitchPort::forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset)
{
    if (packet->srcMac().isConcrete() && !checkMac(packet->srcMac()))
        macList.push_back(packet->srcMac());
    
    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus MacSwitchPort::backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset)
{
    if (prevProcessor != NULL)
        prevProcessor->backwardProcess(proto, packet, offset);
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
/*
bool MacSwitchPort::operator == (const mac_addr & mac)
{
    return checkMac(mac);
}
*/