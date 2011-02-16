#ifndef BRUTEROUTER_H
#define BRUTEROUTER_H

#include <deque>
#include "IRouter.h"
#include "IProcessor.h"

namespace DiplomBukov
{
	class BruteRouter : public IRouter
	{
        typedef std::deque<IProcessor*> MyDeque;

		MyDeque procList;

	public:
        BruteRouter();
        BruteRouter(const MyDeque & d);

		virtual void transmitPacket(Protocol proto, Packet & packet, unsigned offset);
		virtual void addNextProcessor(IProcessor * processor);
		virtual void removeNextProcessor(IProcessor * processor);
		virtual const std::deque<IProcessor*> & nextProcessors();
	};
	// class BruteRouter
}
// namespace DiplomBukov

#endif // BRUTEROUTER_H