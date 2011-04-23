#include "CompositProcessor.h"

using namespace DiplomBukov;

template<typename T>
CompositProcessor<T>::CompositProcessor()
{
    processors.push_back(ProcessorPtr(new T()));
}

template<typename T>
ProcessorPtr CompositProcessor<T>::CreateCopy() const
{
    return processors[0]->CreateCopy();
}

template<typename T>
ProcessorPtr CompositProcessor<T>::getPointer()
{
    return processors[0];
}

template<typename T>
void CompositProcessor<T>::setNextProcessor(ProcessorPtr processor)
{
    processors[0]->setNextProcessor(processor);
}

template<typename T>
ProcessorPtr CompositProcessor<T>::getNextProcessor()
{
    return processors[0]->getNextProcessor();
}

template<typename T>
void CompositProcessor<T>::setPrevProcessor(ProcessorPtr processor)
{
    processors[0]->setPrevProcessor(processor);
}

template<typename T>
ProcessorPtr CompositProcessor<T>::getPrevProcessor()
{
    return processors[0]->getPrevProcessor();
}

template<typename T>
OptionPtr CompositProcessor<T>::getOptions()
{
    return processors[0]->getOptions();
}

template<typename T>
StatsProviderPtr CompositProcessor<T>::statsProvider()
{
    if (processors[0]->statsProvider() == NULL)
        return StatsProviderPtr();
    return StatProviderType::shared_from_this();
}

template<typename T>
std::vector<std::string> CompositProcessor<T>::getStatisticNames() const
{
    if (processors[0]->statsProvider() == NULL)
        return std::vector<std::string>();

    return processors[0]->statsProvider()->getStatisticNames();
}

template<typename T>
std::vector<i64> CompositProcessor<T>::getStatisticValues() const
{
    if (processors[0]->statsProvider() == NULL)
        return std::vector<i64>();

    std::vector<i64> vec;
    for(unsigned i = 0; i < processors.size(); i++)
    {
        std::vector<i64> v = processors[i]->statsProvider()->getStatisticValues();

        if (vec.size() == 0)
            vec.resize(v.size());

        for (unsigned j = 0; j < vec.size(); j++)
            vec[j] += v[j];
    }
    return vec;
}