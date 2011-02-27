#include "BasicLibrary.h"

using namespace DiplomBukov;

BasicLibrary::BasicLibrary()
{
    // Adding all creators to lists
    //
}

const std::deque<IConnectorCreator*> & BasicLibrary::getConnectorCreators()
{
    return ConnectorCreatorList;
}

const std::deque<IAdapterCreator*> & BasicLibrary::getAdapterCreators()
{
    return adapterCreatorList;
}

const std::deque<IProcessorCreator*> & BasicLibrary::getProcessorCreators()
{
    return processorCreatorList;
}