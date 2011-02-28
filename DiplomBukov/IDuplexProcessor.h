#ifndef IDUPLEXPROCESSOR_H
#define IDUPLEXPROCESSOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IDuplexProcessor
    {
    public:
        virtual ~IDuplexProcessor() = 0;

        virtual IProcessorPtr getLeftPointer() = 0;
        virtual IProcessorPtr getRightPointer() = 0;

        virtual void setRightProcessor(IProcessorPtr processor) = 0;
        virtual IProcessorPtr getRightProcessor() = 0;

        virtual void setLeftProcessor(IProcessorPtr processor) = 0;
        virtual IProcessorPtr getLeftProcessor() = 0;
    };
    // class IDuplexProcessor
}
// namespace DiplomBukov

#endif // IDUPLEXPROCESSOR_H