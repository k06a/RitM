#include "stdio.h"
#include "BasicLibrary.h"

using namespace DiplomBukov;

BasicLibrary::BasicLibrary()
{
    // Adding all creators to lists
    //
}

std::deque<IRouterCreator*> & BasicLibrary::getRouterCreators()
{
    return routerCreatorList;
}

std::deque<IAdapterCreator*> & BasicLibrary::getAdapterCreators()
{
    return adapterCreatorList;
}

std::deque<IProcessorCreator*> & BasicLibrary::getProcessorCreators()
{
    return processorCreatorList;
}