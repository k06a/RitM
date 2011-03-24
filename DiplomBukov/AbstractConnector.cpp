#include "AbstractConnector.h"

using namespace DiplomBukov;

AbstractConnector::AbstractConnector()
    : module(NULL)
{
}

void AbstractConnector::DestroyHierarchy()
{
    for(MyDeque::iterator it = procList.begin(); it != procList.end(); ++it)
        (*it)->DestroyHierarchy();
    procList.clear();
    setPrevProcessor(IProcessorPtr());
    setNextProcessor(IProcessorPtr());
    setSelf(IProcessorPtr());
}

void AbstractConnector::setNextProcessor(IProcessorPtr processor)
{
    addNextProcessor(processor);
    AbstractProcessor::setNextProcessor(processor);
}

IProcessorPtr AbstractConnector::getNextProcessor()
{
    return AbstractProcessor::getNextProcessor();
}

void AbstractConnector::ping(IProcessorPtr prevProcessor)
{
    for(MyDeque::iterator it = procList.begin(); it != procList.end(); ++it)
        (*it)->ping(prevProcessor);
    AbstractProcessor::setPrevProcessor(prevProcessor);
}

void AbstractConnector::addNextProcessor(IProcessorPtr processor)
{
    procList.push_back(processor);
}

void AbstractConnector::removeNextProcessor(IProcessorPtr processor)
{
    std::remove(procList.begin(), procList.end(), processor);
}

const AbstractConnector::MyDeque & AbstractConnector::nextProcessors() const
{
    return procList;
}