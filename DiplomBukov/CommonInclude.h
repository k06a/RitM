#include <memory>
#include "Packet.h"
#include "Protocol.h"
#include "ProcessingStatus.h"

#include "i64u64.h"

namespace DiplomBukov
{
    #define extends public
    #define implements public

    class IPacket;
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

    typedef std::tr1::shared_ptr<IPacket> IPacketPtr;
    typedef std::tr1::shared_ptr<IRouter> IRouterPtr;
    typedef std::tr1::shared_ptr<IAdapter> IAdapterPtr;
    typedef std::tr1::shared_ptr<IProcessor> IProcessorPtr;

    typedef std::tr1::shared_ptr<IRouterCreator> IRouterCreatorPtr;
    typedef std::tr1::shared_ptr<IAdapterCreator> IAdapterCreatorPtr;
    typedef std::tr1::shared_ptr<IProcessorCreator> IProcessorCreatorPtr;

    typedef std::tr1::shared_ptr<IRouterModule> IRouterModulePtr;
    typedef std::tr1::shared_ptr<IAdapterModule> IAdapterModulePtr;
    typedef std::tr1::shared_ptr<IProcessorModule> IProcessorModulePtr;

    typedef std::tr1::shared_ptr<IRouterPlugin> IRouterPluginPtr;
    typedef std::tr1::shared_ptr<IAdapterPlugin> IAdapterPluginPtr;
    typedef std::tr1::shared_ptr<IProcessorPlugin> IProcessorPluginPtr;

    typedef std::tr1::shared_ptr<ILibrary> ILibraryPtr;
}