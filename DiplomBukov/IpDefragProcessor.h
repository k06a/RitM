#ifndef IPDEFRAGPROCESSOR_H
#define IPDEFRAGPROCESSOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
	class IpDefragProcessor : public IProcessor
	{
		IRouter * baseRouter;

	public:
		IpDefragProcessor(IRouter * router = NULL);
        virtual IProcessor * CreateCopy() const;

		virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
		virtual void setRouter(IRouter * router);
		virtual IRouter * router();
	};
	// class IpDefragProcessor
}
// namespace DiplomBukov

#endif // IPDEFRAGPROCESSOR_H