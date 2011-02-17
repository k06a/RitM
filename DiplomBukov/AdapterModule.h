#ifndef ADAPTERMODULE_H
#define ADAPTERMODULE_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class AdapterModule
    {
        IAdapter * baseAdapter_;

    public:
        AdapterModule(IAdapter * processor = NULL);

        IAdapter * baseAdapter();
        void setBaseAdapter(IAdapter * adapter);
    };
    // class AdapterModule
}
// namespace DiplomBukov

#endif // ADAPTERMODULE_H