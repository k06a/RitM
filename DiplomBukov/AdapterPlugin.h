#ifndef ADAPTERPLUGIN_H
#define ADAPTERPLUGIN_H

#include "CommonInclude.h"
#include "IAdapterPlugin.h"

namespace DiplomBukov
{
    class AdapterPlugin : public IAdapterPlugin
    {
        IAdapterCreator * adapterCreator;

    public:
        AdapterPlugin(IAdapterCreator * creator = 0);

        virtual IAdapterCreator * getAdapterCreator();
        virtual void setAdapterCreator(IAdapterCreator * creator);
    };
    // class AdapterPlugin
}
// namespace DiplomBukov

#endif // ADAPTERPLUGIN_H