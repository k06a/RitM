#include "BruteRouter.h"
#include "ProcessingStatus.h"
#include <algorithm>

using namespace DiplomBukov;

BruteRouter::BruteRouter()
    : procList()
{

}

BruteRouter::BruteRouter(const MyDeque & d)
    : procList()
{
    for(MyDeque::const_iterator it = d.begin(); it != d.end(); ++it)
    {
        IProcessor * proc = (IProcessor*)(*it)->CreateCopy();
        procList.push_back(proc);
    }
}

IRouter * BruteRouter::CreateCopy() const
{
    return new BruteRouter(procList);
}

IPacketProcessor * BruteRouter::getPointer()
{
    return this;
}

void BruteRouter::ping(IPacketProcessor * prevProcessor)
{

}

ProcessingStatus BruteRouter::forwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    ProcessingStatus ans = ProcessingStatus::Rejected;
	for(MyDeque::iterator it = procList.begin(); it != procList.end(); it++)
    {
		ProcessingStatus ret = (*it)->forwardProcess(proto, packet, offset);
        if (ret == ProcessingStatus::Accepted)
            ans = ProcessingStatus::Accepted;
	}
    return ans;
}

ProcessingStatus BruteRouter::backwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    return ProcessingStatus::Accepted;
}

void BruteRouter::addNextProcessor(IProcessor * processor)
{
	procList.push_back(processor);
}

void BruteRouter::removeNextProcessor(IProcessor * processor)
{
	std::remove(procList.begin(), procList.end(), processor);
}

const std::deque<IProcessor*> & BruteRouter::nextProcessors()
{
	return procList;
}
