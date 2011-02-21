#ifndef BRUTEROUTER_H
#define BRUTEROUTER_H

#include <deque>

#include "AbstractRouter.h"

namespace DiplomBukov
{
	class BruteRouter : public AbstractRouter
	{
    public:
        BruteRouter();
        BruteRouter(const MyDeque & d);
        virtual IRouter * CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
    };
	// class BruteRouter
}
// namespace DiplomBukov

#endif // BRUTEROUTER_H