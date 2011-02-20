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
        IProcessor * proc = (IProcessor*)(*it)->CreateCopy();
        procList.push_back(proc);
        procMap.insert(std::make_pair(proc->getProtocol(), proc));
    }
}

IRouter * ProtocolRouter::CreateCopy() const
{
    return new ProtocolRouter(procList, procMap);
}

ProcessingStatus ProtocolRouter::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    ProcessingStatus ans = ProcessingStatus::Rejected;
    
    for (MyMap::iterator it = procMap.find(proto);
         (it != procMap.end()) && (it->first == proto);
         ++it)
    {
        ProcessingStatus ret = it->second->processPacket(proto, packet, offset);
        if (ret == ProcessingStatus::Accepted)
            ans = ProcessingStatus::Accepted;
    }
    return ans;
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
