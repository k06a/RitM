#ifndef IROUTER_H
#define IROUTER_H

#include <deque>

#include "CommonInclude.h"
#include "IProcessor.h"

namespace DiplomBukov
{
    class IRouter : public virtual IProcessor
	{
	public:
		virtual ~IRouter() {}

        // Cast IProcessor* to IRouterPtr
        virtual IProcessorPtr CreateCopy() const = 0;

        virtual void addNextProcessor(IProcessorPtr processor) = 0;
        virtual void removeNextProcessor(IProcessorPtr processor) = 0;
        virtual const std::deque<IProcessorPtr> & nextProcessors() = 0;
	};
	// class IRouter

    typedef std::tr1::shared_ptr<IRouter> IRouterPtr;
}
// namespace DiplomBukov

#endif //IROUTER_H