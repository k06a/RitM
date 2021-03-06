#include "AbstractConnector.h"

using namespace RitM;

AbstractConnector::AbstractConnector()
    : module(NULL)
{
}

void AbstractConnector::DestroyHierarchy()
{
    for(MyDeque::iterator it = procList.begin(); it != procList.end(); ++it)
        (*it)->DestroyHierarchy();
    procList.clear();
    setPrevProcessor(ProcessorPtr());
    setNextProcessor(ProcessorPtr());
}

void AbstractConnector::setNextProcessor(ProcessorPtr processor)
{
    addNextProcessor(processor);
    AbstractProcessor::setNextProcessor(processor);
}

ProcessorPtr AbstractConnector::getNextProcessor()
{
    return AbstractProcessor::getNextProcessor();
}

void AbstractConnector::ping(ProcessorPtr prevProcessor)
{
    for(MyDeque::iterator it = procList.begin(); it != procList.end(); ++it)
        (*it)->ping(prevProcessor);
    AbstractProcessor::setPrevProcessor(prevProcessor);
}

void AbstractConnector::addNextProcessor(ProcessorPtr processor)
{
    if (std::find(procList.begin(), procList.end(), processor)
        != procList.end())
        return;

    procList.push_back(processor);
}

void AbstractConnector::removeNextProcessor(ProcessorPtr processor)
{
    std::remove(procList.begin(), procList.end(), processor);
}

const AbstractConnector::MyDeque & AbstractConnector::nextProcessors() const
{
    return procList;
}