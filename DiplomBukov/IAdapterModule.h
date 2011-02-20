#ifndef IADAPTERMODULE_H
#define IADAPTERMODULE_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IAdapterModule
    {
    public:
        virtual ~IAdapterModule() {}
        
        virtual IAdapter * getBaseAdapter() = 0;
        virtual void setBaseAdapter(IAdapter * adapter) = 0;

        //virtual IAdapter * getPlugin() = 0;
        //virtual void setPlugin(I * adapter) = 0;
    };
    // class IAdapterModule
}
// namespace DiplomBukov

#endif // IADAPTERMODULE_H