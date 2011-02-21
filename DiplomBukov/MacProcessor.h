#ifndef MACPROCESSOR_H
#define MACPROCESSOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
	class MacProcessor : public IProcessor
	{
        IProcessorModule * module;
        IRouter * router;
        
	public:
		MacProcessor(IRouter * router = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual IPacketProcessor * getPointer();
        virtual void ping(IPacketProcessor * prevProcessor);
        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

		virtual void setRouter(IRouter * router);
		virtual IRouter * getRouter();

        virtual void setModule(IProcessorModule * module);
        virtual IProcessorModule * getModule();
	};
	// class MacProcessor
}
// namespace DiplomBukov

#endif // MACPROCESSOR_H