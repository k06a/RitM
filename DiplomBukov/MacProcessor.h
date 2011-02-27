#ifndef MACPROCESSOR_H
#define MACPROCESSOR_H

#include "CommonInclude.h"
#include "IPacket.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class MacProcessor : public AbstractProcessor
	{
	public:
		MacProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class MacProcessor
}
// namespace DiplomBukov

#endif // MACPROCESSOR_H