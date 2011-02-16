#ifndef IPV4PROCESSOR_H
#define IPV4PROCESSOR_H

#include <map>
#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

#include "ipv4_header.h"

namespace DiplomBukov
{
    class Ipv4Processor : public IProcessor
	{
        typedef std::pair<ipv4_addr,ipv4_addr> ipv4_pair;
        typedef std::map<ipv4_pair,IRouter*> MyMap;
        
		IRouter * baseRouter;
        MyMap routers;

	public:
		Ipv4Processor(IRouter * baseRouter = NULL);
        virtual IProcessor * CreateCopy() const;

		virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
		virtual void setRouter(IRouter * router_);
		virtual IRouter * router();
	};
	// class Ipv4Processor
}
// namespace DiplomBukov

#endif // IPV4PROCESSOR_H