#ifndef IMODULE_H
#define IMODULE_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IModule
    {
    public:
        virtual ~IModule() {}

        virtual ProcessorPtr createProcessor() const = 0;
        virtual std::string info() const = 0;
    };
    // class IModule

    //

    template<typename T>
    class TemplateModule : public IModule
    {
        std::string m_info;
        
    public:
        TemplateModule(const std::string & info)
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

#endif // IMODULE_H