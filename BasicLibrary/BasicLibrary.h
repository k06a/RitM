#ifndef BASICLIBRARY_H
#define BASICLIBRARY_H

#include <deque>
#include "../DiplomBukov/ILibrary.h"

namespace DiplomBukov
{
    class BasicLibrary : public ILibrary
    {
        std::deque<IRouterCreator*> routerCreatorList;
        std::deque<IAdapterCreator*> adapterCreatorList;
        std::deque<IProcessorCreator*> processorCreatorList;
        
    public:
        BasicLibrary();

        virtual const std::deque<IRouterCreator*> & getRouterCreators();
        virtual const std::deque<IAdapterCreator*> & getAdapterCreators();
        virtual const std::deque<IProcessorCreator*> & getProcessorCreators();
    };
    // class BasicLibrary
}
// namespace DiplomBukov

extern "C" __declspec(dllexport)
DiplomBukov::ILibrary * getLibrary()
{
    return new DiplomBukov::BasicLibrary();
}

#endif // BASICLIBRARY_H