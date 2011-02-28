#ifndef DUPLEXPROCESSOR_H
#define DUPLEXPROCESSOR_H

namespace DiplomBukov
{
    class DuplexProcessor : public IDuplexProcessor
    {
        IDuplexProcessorPtr leftProcessor;
        IDuplexProcessorPtr rightProcessor;

        IProcessorPtr leftToRight_Processor;
        IProcessorPtr rightToLeft_Processor;

    public:

        virtual IProcessorPtr getLeftPointer()
        {
            return leftToRight_Processor;
        }

        virtual IProcessorPtr getRightPointer()
        {
            return rightToLeft_Processor;
        }

        virtual void setRightProcessor(IDuplexProcessorPtr processor)
        {
            rightProcessor = processor;
            leftToRight_Processor->setNextProcessor(processor->getLeftPointer());
        }

        virtual IDuplexProcessorPtr getRightProcessor()
        {
            return rightProcessor;
        }

        virtual void setLeftProcessor(IDuplexProcessorPtr processor)
        {
            leftProcessor = processor;
            rightToLeft_Processor->setNextProcessor(processor->getRightPointer());
        }

        virtual IDuplexProcessorPtr getLeftProcessor()
        {
            return leftProcessor;
        }
    };
    // class DuplexProcessor
}
// namespace DiplomBukov

#endif // DUPLEXPROCESSOR_H