#ifndef ILIBRARY_H
#define ILIBRARY_H

#include <deque>
#include "CommonInclude.h"

namespace DiplomBukov
{
    class ILibrary
    {
    public:
        virtual ~ILibrary() {}

        virtual std::deque<IRouterCreator*> & getRouterCreators() = 0;
        virtual std::deque<IAdapterCreator*> & getAdapterCreators() = 0;
        virtual std::deque<IProcessorCreator*> & getProcessorCreators() = 0;
    };
    // class ILibrary
}
// namespace DiplomBukov

#endif // ILIBRARY_H