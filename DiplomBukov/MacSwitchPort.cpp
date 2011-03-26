#include "MacSwitchPort.h"
#include "IPacket.h"

using namespace DiplomBukov;

MacSwitchPort::MacSwitchPort(ProcessorPtr const nextProcessor)
{
    this->nextProcessor = nextProcessor;
    this->prevProcessor = ProcessorPtr();
    macList.push_back(mac_addr::broadcast());
}

MacSwitchPort::MacSwitchPort(const MacSwitchPort & macSwitchPort)
{
    nextProcessor = macSwitchPort.nextProcessor;
    prevProcessor = ProcessorPtr();
}

ProcessorPtr MacSwitchPort::CreateCopy() const
{
    ProcessorPtr ptr(new MacSwitchPort(*this));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus MacSwitchPort::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (packet->srcMac().isConcrete() && !checkMac(packet->srcMac()))
        macList.push_back(packet->srcMac());
    
    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus MacSwitchPort::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
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