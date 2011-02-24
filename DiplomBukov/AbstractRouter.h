#ifndef ABSTRACTROUTER_H
#define ABSTRACTROUTER_H

#include <deque>
#include <algorithm>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IProcessor.h"
#include "IRouter.h"

namespace DiplomBukov
{
    class AbstractRouter
        : public AbstractProcessor
        , public IRouter
    {
    protected:
        typedef std::deque<IProcessorPtr> MyDeque;

        IRouterModule * module;
        MyDeque procList;

    public:
        AbstractRouter::AbstractRouter()
            : module(NULL)
        {
        }

        virtual void setNextProcessor(IProcessorPtr processor)
        {
            addNextProcessor(processor);
            AbstractProcessor::setNextProcessor(processor);
        }

        virtual IProcessorPtr getNextProcessor()
        {
            return AbstractProcessor::getNextProcessor();
        }

        virtual void ping(IProcessorPtr prevProcessor)
        {
            for(MyDeque::iterator it = procList.begin(); it != procList.end(); ++it)
                (*it)->ping(prevProcessor);
        }

        virtual void addNextProcessor(IProcessorPtr processor)
        {
            procList.push_back(processor);
        }

        virtual void removeNextProcessor(IProcessorPtr processor)
        {
            std::remove(procList.begin(), procList.end(), processor);
        }

        virtual const MyDeque & nextProcessors() const
        {
            return procList;
        }
    };
    // class AbstractRouter
}
// namespace DiplomBukov

#endif // ABSTRACTROUTER_H