#ifndef ABSTRACTCONNECTOR_H
#define ABSTRACTCONNECTOR_H

#include <deque>
#include <algorithm>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IProcessor.h"
#include "IConnector.h"

namespace DiplomBukov
{
    class AbstractConnector
        : public AbstractProcessor
        , public IConnector
    {
    protected:
        typedef std::deque<IProcessorPtr> MyDeque;

        IConnectorModule * module;
        MyDeque procList;

    public:
        AbstractConnector::AbstractConnector()
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
    // class AbstractConnector
}
// namespace DiplomBukov

#endif // ABSTRACTCONNECTOR_H