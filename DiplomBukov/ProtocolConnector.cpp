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

ProtocolConnector::ProtocolConnector(const ProtocolConnector & Connector)
    : procMap()
{
    Init(Connector.procList);
}

void ProtocolConnector::Init(const MyDeque & d)
{
    for(MyDeque::const_iterator it = d.begin(); it != d.end(); ++it)
    {
        IProcessorPtr proc = (*it)->CreateCopy();
        procMap.insert(std::make_pair(proc->getProtocol(), proc));
        AbstractConnector::addNextProcessor(proc);
    }
}

IProcessorPtr ProtocolConnector::CreateCopy() const
{
    return IProcessorPtr(new ProtocolConnector(*this));
}

void ProtocolConnector::ping(IProcessorPtr prevProcessor)
{
    setPrevProcessor(prevProcessor);
    for (MyMap::iterator it = procMap.begin(); it != procMap.end(); ++it)
        it->second->ping(prevProcessor);
}

ProcessingStatus ProtocolConnector::forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset)
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

void ProtocolConnector::addNextProcessor(IProcessorPtr processor)
{
    Protocol proto = processor->getProtocol();
    procMap.insert(std::make_pair(proto, processor));
    AbstractConnector::addNextProcessor(processor);
}

void ProtocolConnector::removeNextProcessor(IProcessorPtr processor)
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

const std::deque<IProcessorPtr> & ProtocolConnector::nextProcessors()
{
    return AbstractConnector::nextProcessors();
}
