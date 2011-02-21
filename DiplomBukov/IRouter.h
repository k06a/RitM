#ifndef IROUTER_H
#define IROUTER_H

#include <deque>
#include "CommonInclude.h"
#include "IPacketProcessor.h"

namespace DiplomBukov
{
    class IRouter : public virtual IPacketProcessor
	{
	public:
		virtual ~IRouter() {}

        // Cast IPacketProcessor* to IRouter*
        virtual IRouter * CreateCopy() const = 0;
		
        virtual void setModule(IRouterModule * module) = 0;
        virtual IRouterModule * getModule() = 0;

        virtual void addNextProcessor(IProcessor * processor) = 0;
        virtual void removeNextProcessor(IProcessor * processor) = 0;
        virtual const std::deque<IProcessor*> & nextProcessors() = 0;
	};
	// class IRouter
}
// namespace DiplomBukov

#endif //IROUTER_H