#ifndef IADAPTER_H
#define IADAPTER_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IAdapter : public virtual IProcessor
	{
    public:
        enum Type
        {
            Online,
            Offline
        };

	public:
		virtual ~IAdapter() {}

		virtual void run(bool always) = 0;
        virtual bool tick() = 0;

        virtual Type type() = 0;
	};
	// class IAdapter
}
// namespace DiplomBukov

#endif // IADAPTER_H