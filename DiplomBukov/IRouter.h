#ifndef IROUTER_H
#define IROUTER_H

#include "CommonInclude.h"

namespace DiplomBukov
{
	class IRouter;
	class IAdapter;
	class IProcessor;

	class IRouter
	{
	public:
		virtual ~IRouter() {}

		virtual void transmitPacket(Protocol proto, Packet & packet, unsigned offset) = 0;
		virtual void addNextProcessor(IProcessor * packetProcessor) = 0;
		virtual void removeNextProcessor(IProcessor * packetProcessor) = 0;
	};
	// class IRouter
}
// namespace DiplomBukov

#endif //IROUTER_H