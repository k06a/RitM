#ifndef ISTARTER_H
#define ISTARTER_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IStarter
    {
    public:
        virtual ~IStarter() {}

        virtual void addAdapter(AdapterPtr adapter) = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
    };
    // class IStarter
}
// namespace DiplomBukov

#endif // ISTARTER_H