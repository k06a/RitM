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
		MacHeaderProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class MacHeaderProcessor
}
// namespace DiplomBukov

#endif // MACHEADERPROCESSOR_H