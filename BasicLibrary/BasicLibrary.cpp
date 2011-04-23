#include "BasicLibrary.h"

using namespace DiplomBukov;

BasicLibrary::BasicLibrary()
{
    // Adding all creators to lists
    //
}

std::deque<ConnectorModulePtr> BasicLibrary::getConnectorModules()
{
    return std::deque<ConnectorModulePtr>();
}

std::deque<AdapterModulePtr> BasicLibrary::getAdapterModules()
{
    return std::deque<AdapterModulePtr>();
}

std::deque<ProcessorModulePtr> BasicLibrary::getProcessorModules()
{
    return std::deque<ProcessorModulePtr>();
}