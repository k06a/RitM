#ifndef IMODULE_H
#define IMODULE_H

namespace RitM
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
// namespace RitM

#endif // IMODULE_H