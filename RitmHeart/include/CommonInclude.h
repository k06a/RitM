#pragma once

#include <memory>
#include "Protocol.h"
#include "ProcessingStatus.h"

#include "i64u64.h"

#ifdef RITM_DLLLIB_EXPORTS
#define RITM_API __declspec(dllexport)
#else
#define RITM_API __declspec(dllimport)
#endif

namespace RitM
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

    template <class Ty, class Other>
    std::tr1::shared_ptr<Ty> SharedPointerCast(const std::tr1::shared_ptr<Other> & sp)
    {
        return std::tr1::dynamic_pointer_cast<Ty>(sp);
    }

    class IPacket;
    class IAdapter;
    class IConnector;
    class IProcessor;
    class IStatsProvider;

    class IModule;
    class IAdapterModule;
    class IConnectorModule;
    class IProcessorModule;

    class IOption;
    class IOptionWalker;

    class ILibrary;
    class IStarter;

    typedef SharedPointer<IPacket>::Type             PacketPtr;
    typedef SharedPointer<IAdapter>::Type            AdapterPtr;
    typedef SharedPointer<IConnector>::Type          ConnectorPtr;
    typedef SharedPointer<IProcessor>::Type          ProcessorPtr;
    typedef SharedPointer<IStatsProvider>::Type      StatsProviderPtr;
    
    typedef SharedPointer<IModule>::Type             ModulePtr;
    typedef SharedPointer<IAdapterModule>::Type      AdapterModulePtr;
    typedef SharedPointer<IConnectorModule>::Type    ConnectorModulePtr;
    typedef SharedPointer<IProcessorModule>::Type    ProcessorModulePtr;

    typedef SharedPointer<IOption>::Type             OptionPtr;
    typedef SharedPointer<IOptionWalker>::Type       OptionWalkerPtr;

    typedef SharedPointer<ILibrary>::Type            LibraryPtr;
    typedef SharedPointer<IStarter>::Type            StarterPtr;
}