#ifndef ABSTRACTROUTER_H
#define ABSTRACTROUTER_H

#include <deque>
#include <algorithm>

#include "CommonInclude.h"
#include "AbstractPacketProcessor.h"
#include "IProcessor.h"
#include "IRouter.h"

namespace DiplomBukov
{
    class AbstractRouter
        : public AbstractPacketProcessor
        , public IRouter
    {
    protected:
        typedef std::deque<IProcessor*> MyDeque;

        IRouterModule * module;
        MyDeque procList;

    public:
        AbstractRouter::AbstractRouter()
            : module(NULL)
        {
        }

        virtual void setNextProcessor(IPacketProcessor * processor)
        {
            IProcessor * ip = dynamic_cast<IProcessor*>(processor);
            if (ip != NULL)
                addNextProcessor(ip);
        }

        virtual void ping(IPacketProcessor * prevProcessor)
        {
            for(MyDeque::iterator it = procList.begin(); it != procList.end(); ++it)
                (*it)->ping(prevProcessor);
        }

        virtual void setModule(IRouterModule * mod)
        {
            module = mod;
        }

        virtual IRouterModule * getModule()
        {
            return module;
        }

        virtual void addNextProcessor(IProcessor * processor)
        {
            procList.push_back(processor);
        }

        virtual void removeNextProcessor(IProcessor * processor)
        {
            std::remove(procList.begin(), procList.end(), processor);
        }

        virtual const std::deque<IProcessor*> & nextProcessors()
        {
            return procList;
        }
    };
    // class AbstractRouter
}
// namespace DiplomBukov

#endif // ABSTRACTROUTER_H