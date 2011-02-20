#ifndef IADAPTERPLUGIN_H
#define IADAPTERPLUGIN_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IAdapterPlugin
    {
    public:
        virtual ~IAdapterPlugin() {}

        virtual IAdapterCreator * getAdapterCreator() = 0;
        virtual void setAdapterCreator(IAdapterCreator * creator) = 0;
    };
    // class IAdapterPlugin
}
// namespace DiplomBukov

#endif // IADAPTERPLUGIN_H