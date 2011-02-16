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

void BruteRouter::transmitPacket(Protocol proto, Packet & packet, unsigned offset)
{
	for(MyDeque::iterator it = procList.begin(); it != procList.end(); it++)
    {
		(*it)->processPacket(proto, packet, offset);
	}
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
