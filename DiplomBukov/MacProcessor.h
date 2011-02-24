#ifndef MACPROCESSOR_H
#define MACPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class MacProcessor : public AbstractProcessor
	{
	public:
		MacProcessor(IProcessorPtr router = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class MacProcessor
}
// namespace DiplomBukov

#endif // MACPROCESSOR_H