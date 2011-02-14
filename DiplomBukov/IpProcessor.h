#ifndef IPPROCESSOR_H
#define IPPROCESSOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"

namespace DiplomBukov
{
	class IpProcessor : public IProcessor
	{
		IRouter * router_;

	public:
		IpProcessor(IRouter * router_ = NULL);
		virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
		virtual void setRouter(IRouter * router_);
		virtual IRouter * router();
	};
	// class MacProcessor
}
// namespace DiplomBukov

#endif // IPPROCESSOR_H