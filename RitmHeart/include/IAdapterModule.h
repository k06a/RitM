#ifndef IADAPTERMODULE_H
#define IADAPTERMODULE_H

#include "CommonInclude.h"
#include "IModule.h"

namespace RitM
{
    class IAdapterModule : public IModule
    {
    public:
        virtual ~IAdapterModule() {}
        
        virtual AdapterPtr createAdapter() const = 0;
    };
    // class IAdapterModule
}
// namespace RitM

#endif // IADAPTERMODULE_H