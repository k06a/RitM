#ifndef TEMPLATEPROCESSORMODULE_H
#define TEMPLATEPROCESSORMODULE_H

#include "IProcessorModule.h"

namespace DiplomBukov
{
    template<typename T>
    class TemplateProcessorModule : public IProcessorModule
    {
        std::string m_info;

    public:
        TemplateProcessorModule(const std::string & info)
            : m_info(info)
        {
        }

        virtual ProcessorPtr createProcessor() const
        {
            return ProcessorPtr(new T());
        }

        virtual std::string info() const
        {
            return m_info;
        }
    };
    // class TemplateModule
}
// namespace DiplomBukov

#endif // TEMPLATEPROCESSORMODULE_H