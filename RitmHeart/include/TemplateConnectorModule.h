#ifndef TEMPLATECONNECTORMODULE_H
#define TEMPLATECONNECTORMODULE_H

#include "IConnectorModule.h"

namespace RitM
{
    template<typename T>
    class TemplateConnectorModule : public IConnectorModule
    {
        std::string m_name;
        std::string m_info;

    public:
        TemplateConnectorModule(const std::string & name,
                                const std::string & info)
            : m_name(name), m_info(info)
        {
        }

        virtual ConnectorPtr createConnector() const
        {
            return ConnectorPtr (new T());
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
    // class TemplateConnectorModule
}
// namespace RitM

#endif // TEMPLATECONNECTORMODULE_H