#ifndef IPV4DEFRAGPROCESSOR_H
#define IPV4DEFRAGPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractPacketProcessor.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
	class Ipv4DefragProcessor : public AbstractProcessor
	{
        Packet * fullPacket;
        int ipDataOffset;

	public:
		Ipv4DefragProcessor(IPacketProcessor * router = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class Ipv4DefragProcessor
}
// namespace DiplomBukov

#endif // IPV4DEFRAGPROCESSOR_H