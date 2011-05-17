#ifndef ISTARTER_H
#define ISTARTER_H

#include "CommonInclude.h"

namespace RitM
{
    class IStarter
    {
    public:
        virtual ~IStarter() {}

        virtual void addAdapter(AdapterPtr adapter) = 0;
        virtual void clearAdapters() = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
    };
    // class IStarter
}
// namespace RitM

#endif // ISTARTER_H