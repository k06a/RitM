#ifndef TEMPLATEPROCESSOR_H
#define TEMPLATEPROCESSOR_H

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IStatsProvider.h"

namespace DiplomBukov
{
    template<typename T>
    class CompositProcessor
        : public EnableSharedFromThis<IStatsProvider>::Type
        , public AbstractProcessor
        , public IStatsProvider
    {
        typedef EnableSharedFromThis<IStatsProvider>::Type StatProviderType;

        std::vector<ProcessorPtr> processors;

    public:
        CompositProcessor();
        ProcessorPtr CreateCopy() const;
        virtual ProcessorPtr getPointer();

        virtual void setNextProcessor(ProcessorPtr processor);
        virtual ProcessorPtr getNextProcessor();

        virtual void setPrevProcessor(ProcessorPtr processor);
        virtual ProcessorPtr getPrevProcessor();

        virtual OptionPtr getOptions();

        virtual StatsProviderPtr statsProvider();

        virtual std::vector<std::string> getStatisticNames() const;
        virtual std::vector<i64> getStatisticValues() const;
    };
}
// namespace DiplomBukov

#endif // TEMPLATEPROCESSOR_H