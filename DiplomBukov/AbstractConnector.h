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
        AbstractConnector();
        virtual void DestroyHierarchy();

        virtual void setNextProcessor(IProcessorPtr processor);
        virtual IProcessorPtr getNextProcessor();

        virtual void ping(IProcessorPtr prevProcessor);

        virtual void addNextProcessor(IProcessorPtr processor);
        virtual void removeNextProcessor(IProcessorPtr processor);
        virtual const MyDeque & nextProcessors() const;
    };
    // class AbstractConnector
}
// namespace DiplomBukov

#endif // ABSTRACTCONNECTOR_H