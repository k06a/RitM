#ifndef IADAPTER_H
#define IADAPTER_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IAdapter : public IPacketProcessor
	{
	public:
		virtual ~IAdapter() {}

		virtual void setRouter(IRouter * router) = 0;
		virtual IRouter * getRouter() = 0;
		virtual void run() = 0;

        virtual IPacketProcessor * CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual IPacketProcessor * getPointer();
        //virtual void ping(IPacketProcessor * prevProcessor);

	public:
		void start()
		{
			// Need to be async
			run();
		}
	};
	// class IAdapter
}
// namespace DiplomBukov

#endif // IADAPTER_H