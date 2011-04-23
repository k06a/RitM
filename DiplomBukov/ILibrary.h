#ifndef ILIBRARY_H
#define ILIBRARY_H

#include <deque>
#include "CommonInclude.h"

namespace DiplomBukov
{
    class ILibrary
    {
    public:
        virtual ~ILibrary() {}

        virtual std::deque<ConnectorModulePtr> getConnectorModules() const = 0;
        virtual std::deque<AdapterModulePtr>   getAdapterModules() const = 0;
        virtual std::deque<ProcessorModulePtr> getProcessorModules() const = 0;
    };
    // class ILibrary
}
// namespace DiplomBukov

#endif // ILIBRARY_H