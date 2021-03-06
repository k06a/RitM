#ifndef IADAPTER_H
#define IADAPTER_H

#include "CommonInclude.h"
#include "IProcessor.h"

namespace RitM
{
    class IAdapter : public virtual IProcessor
    {
    public:
        enum Type
        {
            Online,
            Offline
        };

    public:
        virtual ~IAdapter() {}

        virtual void run(bool always) = 0;
        virtual bool tick() = 0;

        virtual Type type() = 0;
    };
    // class IAdapter
}
// namespace RitM

#endif // IADAPTER_H