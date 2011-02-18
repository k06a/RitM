#include "Packet.h"
#include "Protocol.h"
#include "ProcessingStatus.h"

#include "i64u64.h"

#define KB *1024

namespace DiplomBukov
{
    class IRouter;
    class IAdapter;
    class IProcessor;

    class IRouterCreator;
    class IAdapterCreator;
    class IProcessorCreator;

    class IRouterModule;
    class IAdapterModule;
    class IProcessorModule;

    class IRouterPlugin;
    class IAdapterPlugin;
    class IProcessorPlugin;

    class ILibrary;
}