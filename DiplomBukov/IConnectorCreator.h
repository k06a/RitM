#ifndef ICONNECTORCREATOR_H
#define ICONNECTORCREATOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IConnectorCreator
    {
    public:
        virtual ~IConnectorCreator() {}

        virtual ConnectorPtr createConnector() const = 0;
    };
    // class IConnectorCreator
}
// namespace DiplomBukov

#endif // ICONNECTORCREATOR_H