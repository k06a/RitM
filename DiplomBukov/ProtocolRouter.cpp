#include "ProtocolRouter.h"
#include "ProcessingStatus.h"
#include "IProcessor.h"

#include <map>
#include <utility>
#include <algorithm>

using namespace DiplomBukov;

ProtocolRouter::ProtocolRouter()
    : procMap()
{
}

ProtocolRouter::ProtocolRouter(const ProtocolRouter & router)
    : procMap()
{
    Init(router.procList);
}

void ProtocolRouter::Init(const MyDeque & d)
{
    for(MyDeque::const_iterator it = d.begin(); it != d.end(); ++it)
    {
        IProcessorPtr proc = (*it)->CreateCopy();
        procMap.insert(std::make_pair(proc->getProtocol(), proc));
        AbstractRouter::addNextProcessor(proc);
    }
}

IProcessorPtr ProtocolRouter::CreateCopy() const
{
    return IProcessorPtr(new ProtocolRouter(*this));
}

ProcessingStatus ProtocolRouter::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    ProcessingStatus ans = ProcessingStatus::Rejected;
    
    for (MyMap::iterator it = procMap.find(proto);
         (it != procMap.end()) && (it->first == proto);
         ++it)
    {
        ProcessingStatus ret = it->second->forwardProcess(proto, packet, offset);
        if (ret == ProcessingStatus::Accepted)
            ans = ProcessingStatus::Accepted;
    }
    return ans;
}

void ProtocolRouter::addNextProcessor(IProcessorPtr processor)
{
    Protocol proto = processor->getProtocol();
    procMap.insert(std::make_pair(proto, processor));
    AbstractRouter::addNextProcessor(processor);
}

void ProtocolRouter::removeNextProcessor(IProcessorPtr processor)
{
    for(MyMap::iterator it = procMap.begin(); it != procMap.end(); ++it)
    {
        if (it->second == processor)
        {
            procMap.erase(it);
            break;
        }
    }
    AbstractRouter::removeNextProcessor(processor);
}

const std::deque<IProcessorPtr> & ProtocolRouter::nextProcessors()
{
    return AbstractRouter::nextProcessors();
}
