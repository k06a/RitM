#ifndef IMODULE_H
#define IMODULE_H

#include <string>

namespace DiplomBukov
{
    class IModule
    {
    public:
        virtual ~IModule() {}

        virtual const char * name() const = 0;
        virtual const char * info() const = 0;
    };
    // class IModule
}
// namespace DiplomBukov

#endif // IMODULE_H