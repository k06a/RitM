#ifndef IPV4SPLITTERPROCESSOR_H
#define IPV4SPLITTERPROCESSOR_H

#include <map>
#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

#include "ipv4_header.h"

namespace DiplomBukov
{
    class Ipv4SplitterProcessor : public IProcessor
	{
        typedef std::pair<ipv4_addr,ipv4_addr> ipv4_pair;
        typedef std::map<ipv4_pair,IRouter*> MyMap;
        
		IRouter * baseRouter;
        MyMap routers;

	public:
		Ipv4SplitterProcessor(IRouter * baseRouter = NULL);
        virtual IProcessor * CreateCopy() const;

		virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
		virtual void setRouter(IRouter * router_);
		virtual IRouter * router();
	};
	// class Ipv4SplitterProcessor
}
// namespace DiplomBukov

#endif // IPV4SPLITTERPROCESSOR_H