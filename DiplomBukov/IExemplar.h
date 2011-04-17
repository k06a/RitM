#ifndef IEXEMPLAR_H
#define IEXEMPLAR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IExemplar
    {
    public:
        virtual ~IExemplar() {}

        virtual ProcessorPtr getBaseProcessor() = 0;
        virtual void setBaseProcessor(ProcessorPtr processor) = 0;

        virtual void addProcessorCopy(ProcessorPtr processor) = 0;
    };
    // class IExemplar
}
// namespace DiplomBukov

#endif // IEXEMPLAR_H