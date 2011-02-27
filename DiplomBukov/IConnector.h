#ifndef ICONNECTOR_H
#define ICONNECTOR_H

#include <deque>

#include "CommonInclude.h"
#include "IProcessor.h"

namespace DiplomBukov
{
    class IConnector : public virtual IProcessor
	{
	public:
		virtual ~IConnector() {}

        virtual void addNextProcessor(IProcessorPtr processor) = 0;
        virtual void removeNextProcessor(IProcessorPtr processor) = 0;
        virtual const std::deque<IProcessorPtr> & nextProcessors() const = 0;
	};
	// class IConnector
}
// namespace DiplomBukov

#endif // ICONNECTOR_H