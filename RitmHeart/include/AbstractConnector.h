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
        typedef std::deque<ProcessorPtr> MyDeque;

        IConnectorModule * module;
        MyDeque procList;

    public:
        AbstractConnector();
        virtual void DestroyHierarchy();

        virtual void setNextProcessor(ProcessorPtr processor);
        virtual ProcessorPtr getNextProcessor();

        virtual void ping(ProcessorPtr prevProcessor);

        virtual void addNextProcessor(ProcessorPtr processor);
        virtual void removeNextProcessor(ProcessorPtr processor);
        virtual const MyDeque & nextProcessors() const;
    };
    // class AbstractConnector
}
// namespace DiplomBukov

#endif // ABSTRACTCONNECTOR_H