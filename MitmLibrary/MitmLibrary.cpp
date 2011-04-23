#include "MitmLibrary.h"

#include "TemplateConnectorModule.h"
#include "TemplateAdapterModule.h"
#include "TemplateProcessorModule.h"

#include "DnsMessageProcessor.h"
#include "HttpDefragProcessor.h"
#include "HttpSwapProcessor.h"
#include "TelnetSwapper.h"

using namespace DiplomBukov;

std::deque<ConnectorModulePtr> MitmLibrary::getConnectorModules() const
{
    std::deque<ConnectorModulePtr> list;
    return list;
}

std::deque<AdapterModulePtr> MitmLibrary::getAdapterModules() const
{
    std::deque<AdapterModulePtr> list;
    return list;
}

std::deque<ProcessorModulePtr> MitmLibrary::getProcessorModules() const
{
    std::deque<ProcessorModulePtr> list;

    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<DnsMessageProcessor>(
        "ќбработчик, осуществл€ющий подлог данных DNS.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<HttpDefragProcessor>(
        "ќбработчик, осуществл€ющий сборку сообщений HTTP.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<HttpSwapProcessor>(
        "ќбработчик, осуществл€ющий подлог данных HTTP.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<TelnetSwapper>(
        "ќбработчик, осуществл€ющий подлог данных Telnet.")));

    return list;
}