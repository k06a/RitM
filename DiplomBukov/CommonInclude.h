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
    class IConnector;
    class IDuplexConnector;
    class IAdapter;
    class IProcessor;
    class IDuplexProcessor;

    class IConnectorCreator;
    class IAdapterCreator;
    class IProcessorCreator;

    class IConnectorModule;
    class IAdapterModule;
    class IProcessorModule;

    class IConnectorPlugin;
    class IAdapterPlugin;
    class IProcessorPlugin;

    class ILibrary;

    class IOption;

    typedef std::tr1::shared_ptr<IPacket> IPacketPtr;
    typedef std::tr1::shared_ptr<IConnector> IConnectorPtr;
    typedef std::tr1::shared_ptr<IDuplexConnector> IDuplexConnectorPtr;
    typedef std::tr1::shared_ptr<IAdapter> IAdapterPtr;
    typedef std::tr1::shared_ptr<IProcessor> IProcessorPtr;
    typedef std::tr1::shared_ptr<IDuplexProcessor> IDuplexProcessorPtr;
    
    typedef std::tr1::shared_ptr<IConnectorCreator> IConnectorCreatorPtr;
    typedef std::tr1::shared_ptr<IAdapterCreator> IAdapterCreatorPtr;
    typedef std::tr1::shared_ptr<IProcessorCreator> IProcessorCreatorPtr;

    typedef std::tr1::shared_ptr<IConnectorModule> IConnectorModulePtr;
    typedef std::tr1::shared_ptr<IAdapterModule> IAdapterModulePtr;
    typedef std::tr1::shared_ptr<IProcessorModule> IProcessorModulePtr;

    typedef std::tr1::shared_ptr<IConnectorPlugin> IConnectorPluginPtr;
    typedef std::tr1::shared_ptr<IAdapterPlugin> IAdapterPluginPtr;
    typedef std::tr1::shared_ptr<IProcessorPlugin> IProcessorPluginPtr;

    typedef std::tr1::shared_ptr<ILibrary> ILibraryPtr;

    typedef std::tr1::shared_ptr<IOption> IOptionPtr;
}