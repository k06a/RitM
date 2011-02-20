#ifndef ADAPTERMODULE_H
#define ADAPTERMODULE_H

#include "CommonInclude.h"
#include "IAdapterModule.h"

namespace DiplomBukov
{
    class AdapterModule : public IAdapterModule
    {
        IAdapter * baseAdapter;

    public:
        AdapterModule(IAdapter * adapter = NULL);

        virtual IAdapter * getBaseAdapter();
        virtual void setBaseAdapter(IAdapter * adapter);
    };
    // class AdapterModule
}
// namespace DiplomBukov

#endif // ADAPTERMODULE_H