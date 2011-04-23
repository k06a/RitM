#ifndef TEMPLATECONNECTORMODULE_H
#define TEMPLATECONNECTORMODULE_H

#include "IConnectorModule.h"

namespace DiplomBukov
{
    template<typename T>
    class TemplateConnectorModule : public IConnectorModule
    {
        std::string m_info;

    public:
        TemplateConnectorModule(const std::string & info)
            : m_info(info)
        {
        }

        virtual ConnectorPtr createConnector() const
        {
            return ConnectorPtr (new T());
        }

        virtual std::string info() const
        {
            return m_info;
        }
    };
    // class TemplateConnectorModule
}
// namespace DiplomBukov

#endif // TEMPLATECONNECTORMODULE_H