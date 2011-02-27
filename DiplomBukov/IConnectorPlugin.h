#ifndef IConnectorPLUGIN_H
#define IConnectorPLUGIN_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IConnectorPlugin
    {
    public:
        virtual ~IConnectorPlugin() {}

        virtual IConnectorCreator * getConnectorCreator() = 0;
        virtual void setConnectorCreator(IConnectorCreator * creator) = 0;
    };
    // class IConnectorPlugin
}
// namespace DiplomBukov

#endif // IConnectorPLUGIN_H