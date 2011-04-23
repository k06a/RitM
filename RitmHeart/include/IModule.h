#ifndef IMODULE_H
#define IMODULE_H

#include <string>

namespace DiplomBukov
{
    class IModule
    {
    public:
        virtual ~IModule() {}

        virtual std::string info() const = 0;
    };
    // class IModule
}
// namespace DiplomBukov

#endif // IMODULE_H