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
    
}

ProcessingStatus BruteRouter::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    ProcessingStatus ans = ProcessingStatus::Rejected;
	for(MyDeque::iterator it = procList.begin(); it != procList.end(); it++)
    {
		ProcessingStatus ret = (*it)->processPacket(proto, packet, offset);
        if (ret == ProcessingStatus::Accepted)
            ans = ProcessingStatus::Accepted;
	}
    return ans;
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
