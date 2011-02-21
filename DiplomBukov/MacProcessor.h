#ifndef MACPROCESSOR_H
#define MACPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractPacketProcessor.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class MacProcessor : public AbstractProcessor
	{
	public:
		MacProcessor(IPacketProcessor * router = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class MacProcessor
}
// namespace DiplomBukov

#endif // MACPROCESSOR_H