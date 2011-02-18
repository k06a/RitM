#ifndef IADAPTER_H
#define IADAPTER_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IAdapter
	{
	public:
		virtual ~IAdapter() {}

		virtual void setRouter(IRouter * router) = 0;
		virtual IRouter * getRouter() = 0;
		virtual void run() = 0;

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