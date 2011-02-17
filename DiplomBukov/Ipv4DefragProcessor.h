#ifndef IPV4DEFRAGPROCESSOR_H
#define IPV4DEFRAGPROCESSOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
	class Ipv4DefragProcessor : public IProcessor
	{
		IRouter * baseRouter;
        Packet * fullPacket;
        int ipDataOffset;

	public:
		Ipv4DefragProcessor(IRouter * router = NULL);
        virtual IProcessor * CreateCopy() const;

		virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
		virtual void setRouter(IRouter * router);
		virtual IRouter * router();
	};
	// class Ipv4DefragProcessor
}
// namespace DiplomBukov

#endif // IPV4DEFRAGPROCESSOR_H