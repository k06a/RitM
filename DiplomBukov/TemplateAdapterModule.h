#ifndef TEMPLATEADAPTERMODULE_H
#define TEMPLATEADAPTERMODULE_H

#include "IAdapterModule.h"

namespace DiplomBukov
{
    template<typename T>
    class TemplateAdapterModule : public IAdapterModule
    {
        std::string m_info;

    public:
        TemplateAdapterModule(const std::string & info)
            : m_info(info)
        {
        }

        virtual AdapterPtr createAdapter() const
        {
            return AdapterPtr(new T());
        }

        virtual std::string info() const
        {
            return m_info;
        }
    };
    // class TemplateAdapterModule
}
// namespace DiplomBukov

#endif // TEMPLATEADAPTERMODULE_H