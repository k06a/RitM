#ifndef IPPROCESSOR_H
#define IPPROCESSOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
	class IpProcessor : public IProcessor
	{
		IRouter * router_;

	public:
		IpProcessor(IRouter * router_ = NULL);
        virtual IProcessor * CreateCopy() const;

		virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
		virtual void setRouter(IRouter * router_);
		virtual IRouter * router();
	};
	// class IpProcessor
}
// namespace DiplomBukov

#endif // IPPROCESSOR_H