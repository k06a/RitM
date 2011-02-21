#ifndef IADAPTER_H
#define IADAPTER_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IAdapter : public virtual IPacketProcessor
	{
	public:
		virtual ~IAdapter() {}

		virtual void run() = 0;
	};
	// class IAdapter
}
// namespace DiplomBukov

#endif // IADAPTER_H