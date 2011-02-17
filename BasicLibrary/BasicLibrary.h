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

        virtual std::deque<IRouterCreator*> & getRouterCreators();
        virtual std::deque<IAdapterCreator*> & getAdapterCreators();
        virtual std::deque<IProcessorCreator*> & getProcessorCreators();
    };
    // class BasicLibrary
}
// namespace DiplomBukov

extern "C" __declspec(dllexport)
DiplomBukov::ILibrary * getLibrary()
{
    return new DiplomBukov::BasicLibrary();
}
