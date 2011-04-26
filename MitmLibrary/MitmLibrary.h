#ifndef BASICLIBRARY_H
#define BASICLIBRARY_H

#include "CommonInclude.h"
#include "ILibrary.h"

namespace DiplomBukov
{
    class MitmLibrary : public ILibrary
    {
        std::vector<ConnectorModulePtr> list_con;
        std::vector<AdapterModulePtr>   list_ad;
        std::vector<ProcessorModulePtr> list_proc;

    public:
        MitmLibrary();

        virtual int getConnectorModules_size() const = 0;
        virtual int   getAdapterModules_size() const = 0;
        virtual int getProcessorModules_size() const = 0;

        virtual ConnectorModulePtr getConnectorModules_item(int i) const = 0;
        virtual AdapterModulePtr     getAdapterModules_item(int i) const = 0;
        virtual ProcessorModulePtr getProcessorModules_item(int i) const = 0;
    };
    // class MitmLibrary
}
// namespace DiplomBukov

#endif // BASICLIBRARY_H