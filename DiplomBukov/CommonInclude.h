#pragma once

#include <memory>
#include "Protocol.h"
#include "ProcessingStatus.h"

#include "i64u64.h"

namespace DiplomBukov
{
    template<typename T>
    struct SharedPointer
    {
        typedef std::tr1::shared_ptr<T> Type;
    };

    template<typename T>
    struct EnableSharedFromThis
    {
        typedef std::tr1::enable_shared_from_this<T> Type;
    };

    class IPacket;
    class IAdapter;
    class IConnector;
    class IProcessor;
    class IStatsProvider;

    class IModule;
    class IExemplar;
    
    class IAdapterCreator;
    class IConnectorCreator;
    class IProcessorCreator;

    class IAdapterModule;
    class IConnectorModule;
    class IProcessorModule;

    class IAdapterPlugin;
    class IConnectorPlugin;
    class IProcessorPlugin;

    class IOption;
    class IOptionWalker;

    class ILibrary;

    typedef SharedPointer<IPacket>::Type             PacketPtr;
    typedef SharedPointer<IAdapter>::Type            AdapterPtr;
    typedef SharedPointer<IConnector>::Type          ConnectorPtr;
    typedef SharedPointer<IProcessor>::Type          ProcessorPtr;
    typedef SharedPointer<IStatsProvider>::Type      StatsProviderPtr;
    
    typedef SharedPointer<IModule>::Type             ModulePtr;
    typedef SharedPointer<IExemplar>::Type           ExemplarPtr;
    
    typedef SharedPointer<IAdapterCreator>::Type     AdapterCreatorPtr;
    typedef SharedPointer<IConnectorCreator>::Type   ConnectorCreatorPtr;
    typedef SharedPointer<IProcessorCreator>::Type   ProcessorCreatorPtr;

    typedef SharedPointer<IAdapterModule>::Type      AdapterModulePtr;
    typedef SharedPointer<IConnectorModule>::Type    ConnectorModulePtr;
    typedef SharedPointer<IProcessorModule>::Type    ProcessorModulePtr;

    typedef SharedPointer<IAdapterPlugin>::Type      AdapterPluginPtr;
    typedef SharedPointer<IConnectorPlugin>::Type    ConnectorPluginPtr;
    typedef SharedPointer<IProcessorPlugin>::Type    ProcessorPluginPtr;

    typedef SharedPointer<IOption>::Type             OptionPtr;
    typedef SharedPointer<IOptionWalker>::Type       OptionWalkerPtr;

    typedef SharedPointer<ILibrary>::Type            LibraryPtr;
}