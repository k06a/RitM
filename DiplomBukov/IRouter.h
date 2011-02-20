#ifndef IROUTER_H
#define IROUTER_H

#include <deque>
#include "CommonInclude.h"
#include "IPacketProcessor.h"

namespace DiplomBukov
{
    class IRouter : public IPacketProcessor
	{
	public:
		virtual ~IRouter() {}

        virtual IRouter * CreateCopy() const = 0;
		//virtual void processPacket(Protocol proto, Packet & packet, unsigned offset) = 0;

		virtual void addNextProcessor(IProcessor * packetProcessor) = 0;
		virtual void removeNextProcessor(IProcessor * packetProcessor) = 0;
		virtual const std::deque<IProcessor*> & nextProcessors() = 0;
	};
	// class IRouter
}
// namespace DiplomBukov

#endif //IROUTER_H