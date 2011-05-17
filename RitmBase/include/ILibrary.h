#ifndef ILIBRARY_H
#define ILIBRARY_H

#include <vector>
#include "CommonInclude.h"

namespace RitM
{
    class ILibrary
    {
    public:
        virtual ~ILibrary() {}

        virtual int getConnectorModules_size() const = 0;
        virtual int   getAdapterModules_size() const = 0;
        virtual int getProcessorModules_size() const = 0;

        virtual ConnectorModulePtr getConnectorModules_item(int i) const = 0;
        virtual AdapterModulePtr     getAdapterModules_item(int i) const = 0;
        virtual ProcessorModulePtr getProcessorModules_item(int i) const = 0;
    };
    // class ILibrary
}
// namespace RitM

typedef RitM::ILibrary * (__cdecl *TCreateLibrary)(void);
typedef void (__cdecl *TDeleteLibrary)(RitM::ILibrary *);

#endif // ILIBRARY_H