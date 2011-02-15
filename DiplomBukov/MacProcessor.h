#ifndef MACPROCESSOR_H
#define MACPROCESSOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"

namespace DiplomBukov
{
	class MacProcessor : public IProcessor
	{
		IRouter * router_;

	public:
		MacProcessor(IRouter * router_ = NULL);

		virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
		virtual void setRouter(IRouter * router_);
		virtual IRouter * router();
	};
	// class MacProcessor
}
// namespace DiplomBukov

#endif // MACPROCESSOR_H