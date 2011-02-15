#ifndef IPROCESSOR_H
#define IPROCESSOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
	class IRouter;
	class IAdapter;
	class IProcessor;

	class IProcessor
	{
	public:
		virtual ~IProcessor() {}

		virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset) = 0;
        virtual Protocol getProtocol() = 0;
		virtual void setRouter(IRouter * adapter) = 0;
		virtual IRouter * router() = 0;
	};
	// class IProcessor
}
// namespace DiplomBukov

#endif // IPROCESSOR_H