#include "ProtocolRouter.h"
#include "ProcessingStatus.h"

#include <map>
#include <utility>
#include <algorithm>

using namespace DiplomBukov;

void ProtocolRouter::transmitPacket(Protocol proto, Packet & packet, unsigned offset)
{
    std::multimap<Protocol,IProcessor*>::iterator it = mapProcessors.find(proto);
    while ((it != mapProcessors.end()) && (it->first == proto))
    {
        it->second->processPacket(proto, packet, offset);
        ++it;
    }
}

void ProtocolRouter::addNextProcessor(IProcessor * processor)
{
    Protocol proto = processor->getProtocol();
    nextProcessors.push_back(processor);
    mapProcessors.insert(std::make_pair(proto, processor));
}

void ProtocolRouter::removeNextProcessor(IProcessor * processor)
{
    std::remove(nextProcessors.begin(), nextProcessors.end(), processor);
    for(std::multimap<Protocol,IProcessor*>::iterator
        it = mapProcessors.begin();
        it != mapProcessors.end();
        ++it)
    {
        if (it->second == processor)
        {
            mapProcessors.erase(it);
            break;
        }
    }
}

const std::deque<IProcessor*> & ProtocolRouter::processors()
{
    return nextProcessors;
}
