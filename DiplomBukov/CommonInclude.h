#include "Packet.h"
#include "Protocol.h"
#include "ProcessingStatus.h"

#define KB *1024

typedef signed __int8   i8;
typedef signed __int16 i16;
typedef signed __int32 i32;
typedef signed __int64 i64;

typedef unsigned __int8   u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

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