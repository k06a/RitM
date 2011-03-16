#include "MacSwitch.h"
#include "MacSwitchPort.h"

using namespace DiplomBukov;

MacSwitch::MacSwitch(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

MacSwitch::MacSwitch(const MacSwitch & macSwitch)
{
    for(MyDeque::const_iterator it = macSwitch.ports.begin();
        it != macSwitch.ports.end(); ++it)
    {
        ports.push_back((*it)->CreateCopy());
    }
}

IProcessorPtr MacSwitch::CreateCopy() const
{
    IProcessorPtr ptr(new MacSwitch(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

IProcessorPtr MacSwitch::getPointer()
{
    IProcessorPtr port(new MacSwitchPort(Self));
    port->setSelf(port);
    ports.push_back(port);
    return port;
}

ProcessingStatus MacSwitch::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    if (packet->status() == IPacket::Accepted)
        backwardProcess(packet->format(), packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus MacSwitch::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    int count = 0;
    for(MyDeque::iterator it = ports.begin(); it != ports.end(); ++it)
    {
        MacSwitchPort * port = dynamic_cast<MacSwitchPort *>(it->get());
        if (port->checkMac(packet->dst_mac()))
        {
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