#ifndef BASICLIBRARY_H
#define BASICLIBRARY_H

#include "CommonInclude.h"
#include "ILibrary.h"

namespace RitM
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
// namespace RitM

extern "C" __declspec(dllexport)
RitM::ILibrary * createLibrary()
{
    return new RitM::BasicLibrary();
}

extern "C" __declspec(dllexport)
void deleteLibrary(RitM::ILibrary * lib)
{
    delete lib;
}

#endif // BASICLIBRARY_H