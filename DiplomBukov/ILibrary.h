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

        virtual const std::deque<IConnectorCreator*> & getConnectorCreators() = 0;
        virtual const std::deque<IAdapterCreator*> & getAdapterCreators() = 0;
        virtual const std::deque<IProcessorCreator*> & getProcessorCreators() = 0;
    };
    // class ILibrary
}
// namespace DiplomBukov

#endif // ILIBRARY_H