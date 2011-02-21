#ifndef IADAPTERCREATOR_H
#define IADAPTERCREATOR_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IAdapterCreator
    {
    public:
        virtual ~IProcessorCreator() {}

        virtual IAdapter * createAdapter(IRouter * router = NULL) const = 0;
    };
    // class IAdapterCreator
}
// namespace DiplomBukov

#endif // IADAPTERCREATOR_H