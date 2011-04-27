#ifndef TEMPLATEADAPTERMODULE_H
#define TEMPLATEADAPTERMODULE_H

#include "IAdapterModule.h"

namespace DiplomBukov
{
    template<typename T>
    class TemplateAdapterModule : public IAdapterModule
    {
        std::string m_name;
        std::string m_info;

    public:
        TemplateAdapterModule(const std::string & name,
                              const std::string & info)
            : m_name(name), m_info(info)
        {
        }

        virtual AdapterPtr createAdapter() const
        {
            return AdapterPtr(new T());
        }

        virtual const char * name() const
        {
            return m_name.c_str();
        }

        virtual const char * info() const
        {
            return m_info.c_str();
        }
    };
    // class TemplateAdapterModule
}
// namespace DiplomBukov

#endif // TEMPLATEADAPTERMODULE_H