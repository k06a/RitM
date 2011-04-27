#ifndef BASICLIBRARY_H
#define BASICLIBRARY_H

#include "CommonInclude.h"
#include "ILibrary.h"

namespace DiplomBukov
{
    class BasicLibrary : public ILibrary
    {
        std::vector<ConnectorModulePtr> list_con;
        std::vector<AdapterModulePtr>   list_ad;
        std::vector<ProcessorModulePtr> list_proc;

    public:
        BasicLibrary();

        virtual int getConnectorModules_size() const;
        virtual int   getAdapterModules_size() const;
        virtual int getProcessorModules_size() const;

        virtual ConnectorModulePtr getConnectorModules_item(int i) const;
        virtual AdapterModulePtr     getAdapterModules_item(int i) const;
        virtual ProcessorModulePtr getProcessorModules_item(int i) const;
    };
    // class BasicLibrary
}
// namespace DiplomBukov

extern "C" __declspec(dllexport)
DiplomBukov::ILibrary * createLibrary()
{
    return new DiplomBukov::BasicLibrary();
}

extern "C" __declspec(dllexport)
void deleteLibrary(DiplomBukov::ILibrary * lib)
{
    delete lib;
}

#endif // BASICLIBRARY_H