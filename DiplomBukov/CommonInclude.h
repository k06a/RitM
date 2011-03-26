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

    typedef SmartPointer<IPacket>::Type IPacketPtr;
    typedef SmartPointer<IConnector>::Type IConnectorPtr;
    typedef SmartPointer<IDuplexConnector>::Type IDuplexConnectorPtr;
    typedef SmartPointer<IAdapter>::Type IAdapterPtr;
    typedef SmartPointer<IProcessor>::Type IProcessorPtr;
    typedef SmartPointer<IDuplexProcessor>::Type IDuplexProcessorPtr;
    
    typedef SmartPointer<IConnectorCreator>::Type IConnectorCreatorPtr;
    typedef SmartPointer<IAdapterCreator>::Type IAdapterCreatorPtr;
    typedef SmartPointer<IProcessorCreator>::Type IProcessorCreatorPtr;

    typedef SmartPointer<IConnectorModule>::Type IConnectorModulePtr;
    typedef SmartPointer<IAdapterModule>::Type IAdapterModulePtr;
    typedef SmartPointer<IProcessorModule>::Type IProcessorModulePtr;

    typedef SmartPointer<IConnectorPlugin>::Type IConnectorPluginPtr;
    typedef SmartPointer<IAdapterPlugin>::Type IAdapterPluginPtr;
    typedef SmartPointer<IProcessorPlugin>::Type IProcessorPluginPtr;

    typedef SmartPointer<ILibrary>::Type ILibraryPtr;

    typedef SmartPointer<IOption>::Type IOptionPtr;
}