#include "MacSwitch.h"
#include "MacSwitchPort.h"

using namespace DiplomBukov;

MacSwitch::MacSwitch()
    : router()
{
}

MacSwitch::MacSwitch(const MacSwitch & macSwitch)
    : router(macSwitch.router)
{
}

IRouter * MacSwitch::CreateCopy() const
{
    return new MacSwitch(*this);
}

IPacketProcessor * MacSwitch::getPointer()
{
    MacSwitchPort * port = new MacSwitchPort(this);
    ports.push_back(port);
    return port;
}

void MacSwitch::ping(IPacketProcessor * prevProcessor)
{
    router.ping(this);
}

ProcessingStatus MacSwitch::forwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    return router.forwardProcess(proto, packet, offset);
}

ProcessingStatus MacSwitch::backwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    //
    return ProcessingStatus::Accepted;
}

void MacSwitch::addNextProcessor(IProcessor * packetProcessor)
{
    router.addNextProcessor(packetProcessor);
}

void MacSwitch::removeNextProcessor(IProcessor * packetProcessor)
{
    router.removeNextProcessor(packetProcessor);
}

const std::deque<IProcessor*> & MacSwitch::nextProcessors()
{
    return router.nextProcessors();
}