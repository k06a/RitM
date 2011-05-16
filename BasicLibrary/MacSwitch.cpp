#include "MacSwitch.h"
#include "MacSwitchPort.h"

using namespace RitM;

MacSwitch::MacSwitch(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

MacSwitch::MacSwitch(const MacSwitch & macSwitch)
{
    for(MyDeque::const_iterator it = macSwitch.ports.begin();
        it != macSwitch.ports.end(); ++it)
    {
        MacSwitchPortPtr ptr(new MacSwitchPort(*it));
        ports.push_back(ptr);
    }
}

ProcessorPtr MacSwitch::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new MacSwitch(np));
}

void MacSwitch::DestroyHierarchy()
{
    ports.clear();
    AbstractProcessor::DestroyHierarchy();
}

ProcessorPtr MacSwitch::getPointer()
{
    MacSwitchPortPtr port(new MacSwitchPort(shared_from_this()));
    ports.push_back(port);
    return port;
}

ProcessingStatus MacSwitch::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    packet->addProcessor(shared_from_this());
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    //if (packet->status() == IPacket::Accepted)
    //    backwardProcess(packet->format(), packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus MacSwitch::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    int count = 0;
    for(MyDeque::iterator it = ports.begin(); it != ports.end(); ++it)
    {
        MacSwitchPortPtr port = (*it);
        if (port->checkMac(packet->dstMac()))
        {
            // Не отправлять обратно широковещательные пакеты
            if ((packet->haveProcessor(*it)) && packet->dstMac().isBroadcast())
                continue;

            port->backwardProcess(proto, packet, offset);  
            count++;
        }
    }
    
    // If was sent to nobody, then send to all
    if (count == 0)
    {
        for(MyDeque::iterator it = ports.begin(); it != ports.end(); ++it)
        {
            // Не отправлять обратно
            if (packet->haveProcessor(*it))
                continue;

            (*it)->backwardProcess(proto, packet, offset);
        }
    }
    
    return ProcessingStatus::Accepted;
}

const char * MacSwitch::getProcessorName()
{
    return "MacSwitch";
}