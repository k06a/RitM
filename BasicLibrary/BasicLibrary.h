#ifndef BASICLIBRARY_H
#define BASICLIBRARY_H

#include <deque>
#include "../DiplomBukov/ILibrary.h"
#include "../DiplomBukov/IConnectorModule.h"
#include "../DiplomBukov/IAdapterModule.h"
#include "../DiplomBukov/IProcessorModule.h"

namespace DiplomBukov
{
    class BasicLibrary : public ILibrary
    {
    public:
        BasicLibrary();

        virtual std::deque<ConnectorModulePtr> getConnectorModules();
        virtual std::deque<AdapterModulePtr> getAdapterModules();
        virtual std::deque<ProcessorModulePtr> getProcessorModules();
    };
    // class BasicLibrary
}
// namespace DiplomBukov

extern "C" __declspec(dllexport)
DiplomBukov::ILibrary * getLibrary()
{
    return NULL;//new DiplomBukov::BasicLibrary();
}

#endif // BASICLIBRARY_H