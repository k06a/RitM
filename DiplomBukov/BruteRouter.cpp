#include "BruteRouter.h"
#include "ProcessingStatus.h"
#include <algorithm>

using namespace DiplomBukov;

void BruteRouter::transmitPacket(Protocol proto, Packet & packet, unsigned offset)
{
	for(std::deque<IProcessor*>::iterator
		it = nextProcessors.begin();
		it != nextProcessors.end(); it++)
	{
		(*it)->processPacket(proto, packet, offset);
	}
}

void BruteRouter::addNextProcessor(IProcessor * processor)
{
	nextProcessors.push_back(processor);
}

void BruteRouter::removeNextProcessor(IProcessor * processor)
{
	std::remove(nextProcessors.begin(), nextProcessors.end(), processor);
}

const std::deque<IProcessor*> & BruteRouter::processors()
{
	return nextProcessors;
}
