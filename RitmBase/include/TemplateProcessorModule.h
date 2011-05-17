#ifndef TEMPLATEPROCESSORMODULE_H
#define TEMPLATEPROCESSORMODULE_H

#include "IProcessorModule.h"

namespace RitM
{
    template<typename T, bool isFinal_value = false>
    class TemplateProcessorModule : public IProcessorModule
    {
        std::string m_name;
        std::string m_info;

    public:
        TemplateProcessorModule(const std::string & name,
                                const std::string & info)
            : m_name(name), m_info(info)
        {
        }

        virtual ProcessorPtr createProcessor() const
        {
            return ProcessorPtr(new T());
        }

        virtual const char * name() const
        {
            return m_name.c_str();
        }

        virtual const char * info() const
        {
            return m_info.c_str();
        }

        virtual bool isFinal()
        {
            return isFinal_value;
        }
    };
    // class TemplateModule
}
// namespace RitM

#endif // TEMPLATEPROCESSORMODULE_H