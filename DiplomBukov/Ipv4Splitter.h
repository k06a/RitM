#ifndef IPV4SPLITTERPROCESSOR_H
#define IPV4SPLITTERPROCESSOR_H

#include <map>
#include <utility>

#include "CommonInclude.h"
#include "IPacket.h"
#include "AbstractProcessor.h"
#include "ipv4_header.h"

namespace DiplomBukov
{
    class Ipv4Splitter : public AbstractProcessor
	{
        typedef std::pair<ipv4_addr,ipv4_addr> ipv4_pair;
        typedef std::map<ipv4_pair,IProcessorPtr> MyMap;

        MyMap Connectors;
		
	public:
		Ipv4Splitter(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class Ipv4Splitter
}
// namespace DiplomBukov

#endif // IPV4SPLITTERPROCESSOR_H