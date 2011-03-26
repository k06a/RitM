#ifndef IDUPLEXPROCESSOR_H
#define IDUPLEXPROCESSOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IDuplexProcessor
    {
    public:
        virtual ~IDuplexProcessor() = 0;

        virtual ProcessorPtr getLeftPointer() = 0;
        virtual ProcessorPtr getRightPointer() = 0;

        virtual void setRightProcessor(ProcessorPtr processor) = 0;
        virtual ProcessorPtr getRightProcessor() = 0;

        virtual void setLeftProcessor(ProcessorPtr processor) = 0;
        virtual ProcessorPtr getLeftProcessor() = 0;
    };
    // class IDuplexProcessor
}
// namespace DiplomBukov

#endif // IDUPLEXPROCESSOR_H