#ifndef ICONNECTOR_H
#define ICONNECTOR_H

#include <deque>

#include "CommonInclude.h"
#include "IProcessor.h"

namespace RitM
{
    class IConnector : public virtual IProcessor
    {
    public:
        virtual ~IConnector() {}

        virtual void addNextProcessor(ProcessorPtr processor) = 0;
        virtual void removeNextProcessor(ProcessorPtr processor) = 0;
        virtual const std::deque<ProcessorPtr> & nextProcessors() const = 0;
    };
    // class IConnector
}
// namespace RitM

#endif // ICONNECTOR_H