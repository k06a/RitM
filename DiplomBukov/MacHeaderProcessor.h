#ifndef MACHEADERPROCESSOR_H
#define MACHEADERPROCESSOR_H

#include "CommonInclude.h"
#include "IPacket.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class MacHeaderProcessor : public AbstractProcessor
	{
        mac_header header;

	public:
		MacHeaderProcessor(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class MacHeaderProcessor
}
// namespace DiplomBukov

#endif // MACHEADERPROCESSOR_H