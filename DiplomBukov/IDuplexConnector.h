#ifndef IDUPLEXCONNECTOR_H
#define IDUPLEXCONNECTOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IDuplexConnector
    {
    public:
        virtual ~IDuplexConnector() {}

        virtual void addNextProcessor(IDuplexProcessorPtr processor) = 0;
        virtual void removeNextProcessor(IDuplexProcessorPtr processor) = 0;
        virtual const std::deque<IDuplexProcessorPtr> &
            removeNextProcessor(IDuplexProcessorPtr processor) const = 0;
    };
    // class IDuplexConnector
}
// namespace DiplomBukov

#endif // IDUPLEXCONNECTOR_H