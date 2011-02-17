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
		IRouter * router_;

	public:
		MacProcessor(IRouter * router_ = NULL);
        virtual IProcessor * CreateCopy() const;

		virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
		virtual void setRouter(IRouter * router_);
		virtual IRouter * router();
	};
	// class MacProcessor
}
// namespace DiplomBukov

#endif // MACPROCESSOR_H