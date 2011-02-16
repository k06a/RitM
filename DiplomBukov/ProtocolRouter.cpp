#include "ProtocolRouter.h"
#include "ProcessingStatus.h"

#include <map>
#include <utility>
#include <algorithm>

using namespace DiplomBukov;

ProtocolRouter::ProtocolRouter()
    : procList(), procMap()
{
}

ProtocolRouter::ProtocolRouter(const MyDeque & d, const MyMap m)
    : procList(), procMap()
{
    for(MyDeque::const_iterator it = d.begin(); it != d.end(); ++it)
    {
        IProcessor * proc = (*it)->CreateCopy();
        procList.push_back(proc);
        procMap.insert(std::make_pair(proc->getProtocol(), proc));
    }
}

IRouter * ProtocolRouter::CreateCopy()
{
    return new ProtocolRouter(procList, procMap);
}

void ProtocolRouter::transmitPacket(Protocol proto, Packet & packet, unsigned offset)
{
    MyMap::iterator it = procMap.find(proto);
    while ((it != procMap.end()) && (it->first == proto))
    {
        it->second->processPacket(proto, packet, offset);
        ++it;
    }
}

void ProtocolRouter::addNextProcessor(IProcessor * processor)
{
    Protocol proto = processor->getProtocol();
    procList.push_back(processor);
    procMap.insert(std::make_pair(proto, processor));
}

void ProtocolRouter::removeNextProcessor(IProcessor * processor)
{
    for(MyMap::iterator it = procMap.begin(); it != procMap.end(); ++it)
    {
        if (it->second == processor)
        {
            procMap.erase(it);
            break;
        }
    }
    std::remove(procList.begin(), procList.end(), processor);
}

const std::deque<IProcessor*> & ProtocolRouter::nextProcessors()
{
    return procList;
}
