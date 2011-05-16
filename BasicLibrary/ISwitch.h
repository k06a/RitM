#ifndef ISWITCH_H
#define ISWITCH_H

#include "CommonInclude.h"

namespace RitM
{
    class ISwitch
    {
    public:
        virtual ~ISwitch() {}

        virtual void setRouter(IRouter * router) = 0;
        virtual IRouter * getRouter() = 0;
    };
    // class ISwitch
}
// namespace RitM

#endif // ISWITCH_H