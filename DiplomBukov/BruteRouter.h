#ifndef BRUTEROUTER_H
#define BRUTEROUTER_H

#include <deque>
#include "IRouter.h"
#include "IProcessor.h"

namespace DiplomBukov
{
	class BruteRouter : public IRouter
	{
		std::deque<IProcessor*> nextProcessors;

	public:
		virtual void transmitPacket(Protocol proto, Packet & packet, unsigned offset);
		virtual void addNextProcessor(IProcessor * processor);
		virtual void removeNextProcessor(IProcessor * processor);
	};
	// class BruteRouter
}
// namespace DiplomBukov

#endif // BRUTEROUTER_H