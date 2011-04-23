#ifndef BASICLIBRARY_H
#define BASICLIBRARY_H

#include "CommonInclude.h"
#include "ILibrary.h"

namespace DiplomBukov
{
    class BasicLibrary : public ILibrary
    {
    public:
        virtual std::deque<ConnectorModulePtr> getConnectorModules() const;
        virtual std::deque<AdapterModulePtr>   getAdapterModules() const;
        virtual std::deque<ProcessorModulePtr> getProcessorModules() const;
    };
    // class BasicLibrary
}
// namespace DiplomBukov

#endif // BASICLIBRARY_H