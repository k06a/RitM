#include "ProtocolConnector.h"
#include "ProcessingStatus.h"
#include "IProcessor.h"

#include <map>
#include <utility>
#include <algorithm>

using namespace DiplomBukov;

ProtocolConnector::ProtocolConnector()
    : procMap()
{
}

ProtocolConnector::ProtocolConnector(const MyDeque & d)
    : procMap()
{
    for(MyDeque::const_iterator it = d.begin(); it != d.end(); ++it)
    {
        ProcessorPtr proc = (*it)->CreateCopy();
        procMap.insert(std::make_pair(proc->getProtocol(), proc));
        AbstractConnector::addNextProcessor(proc);
    }
}

ProcessorPtr ProtocolConnector::CreateCopy() const
{
    return ProcessorPtr(new ProtocolConnector(procList));
}

void ProtocolConnector::ping(ProcessorPtr prevProcessor)
{
    setPrevProcessor(prevProcessor);
    for (MyMap::iterator it = procMap.begin(); it != procMap.end(); ++it)
        it->second->ping(prevProcessor);
}

ProcessingStatus ProtocolConnector::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
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

    if ((proto != Protocol::None) && (ans != ProcessingStatus::Accepted))
    {
        for (MyMap::iterator it = procMap.find(Protocol::None);
            (it != procMap.end()) && (it->first == Protocol::None);
            ++it)
        {
            ProcessingStatus ret = it->second->forwardProcess(proto, packet, offset);
            if (ret == ProcessingStatus::Accepted)
                ans = ProcessingStatus::Accepted;
        }
    }

    return ans;
}

void ProtocolConnector::addNextProcessor(ProcessorPtr processor)
{
    if (processor == NULL)
    {
        procMap.clear();
        procList.clear();
        return;
    }

    if (std::find(procList.begin(), procList.end(), processor)
        != procList.end())
        return;

    Protocol proto = processor->getProtocol();
    procMap.insert(std::make_pair(proto, processor));
    AbstractConnector::addNextProcessor(processor);
}

void ProtocolConnector::removeNextProcessor(ProcessorPtr processor)
{
    for(MyMap::iterator it = procMap.begin(); it != procMap.end(); ++it)
    {
        if (it->second == processor)
        {
            procMap.erase(it);
            break;
        }
    }
    AbstractConnector::removeNextProcessor(processor);
}

const std::deque<ProcessorPtr> & ProtocolConnector::nextProcessors()
{
    return AbstractConnector::nextProcessors();
}
