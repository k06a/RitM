#ifndef ADAPTERPLUGIN_H
#define ADAPTERPLUGIN_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class AdapterPlugin
    {
    public:
        AdapterPlugin(IAdapterCreator * adapterCreator = 0);

        IAdapterCreator * adapterCreator();
        void setAdapterCreator(IAdapterCreator * adapterCreator);
    };
    // class AdapterPlugin
}
// namespace DiplomBukov

#endif // ADAPTERPLUGIN_H