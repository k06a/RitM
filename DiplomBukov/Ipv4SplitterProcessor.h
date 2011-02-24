#ifndef IPV4SPLITTERPROCESSOR_H
#define IPV4SPLITTERPROCESSOR_H

#include <map>
#include <utility>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "ipv4_header.h"

namespace DiplomBukov
{
    class Ipv4SplitterProcessor : public AbstractProcessor
	{
        typedef std::pair<ipv4_addr,ipv4_addr> ipv4_pair;
        typedef std::map<ipv4_pair,IProcessorPtr> MyMap;

        MyMap routers;
		
	public:
		Ipv4SplitterProcessor(IProcessorPtr router = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class Ipv4SplitterProcessor
}
// namespace DiplomBukov

#endif // IPV4SPLITTERPROCESSOR_H