#pragma once

#include <memory>
#include "Protocol.h"
#include "ProcessingStatus.h"

#include "i64u64.h"

namespace DiplomBukov
{
    template<typename T>
    struct SmartPointer
    {
        typedef std::tr1::shared_ptr<T> Type;
    };

    template<typename T>
    struct EnableSmartFromThis
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

    typedef SmartPointer<IPacket>::Type             PacketPtr;
    typedef SmartPointer<IAdapter>::Type            AdapterPtr;
    typedef SmartPointer<IConnector>::Type          ConnectorPtr;
    typedef SmartPointer<IProcessor>::Type          ProcessorPtr;
    typedef SmartPointer<IStatsProvider>::Type      StatsProviderPtr;
    
    typedef SmartPointer<IModule>::Type             ModulePtr;
    typedef SmartPointer<IExemplar>::Type           ExemplarPtr;
    
    typedef SmartPointer<IAdapterCreator>::Type     AdapterCreatorPtr;
    typedef SmartPointer<IConnectorCreator>::Type   ConnectorCreatorPtr;
    typedef SmartPointer<IProcessorCreator>::Type   ProcessorCreatorPtr;

    typedef SmartPointer<IAdapterModule>::Type      AdapterModulePtr;
    typedef SmartPointer<IConnectorModule>::Type    ConnectorModulePtr;
    typedef SmartPointer<IProcessorModule>::Type    ProcessorModulePtr;

    typedef SmartPointer<IAdapterPlugin>::Type      AdapterPluginPtr;
    typedef SmartPointer<IConnectorPlugin>::Type    ConnectorPluginPtr;
    typedef SmartPointer<IProcessorPlugin>::Type    ProcessorPluginPtr;

    typedef SmartPointer<IOption>::Type             OptionPtr;
    typedef SmartPointer<IOptionWalker>::Type       OptionWalkerPtr;

    typedef SmartPointer<ILibrary>::Type            LibraryPtr;
}