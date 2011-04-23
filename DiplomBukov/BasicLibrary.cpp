#include "BasicLibrary.h"

#include "TemplateConnectorModule.h"
#include "TemplateAdapterModule.h"
#include "TemplateProcessorModule.h"

#include "BruteConnector.h"
#include "ProtocolConnector.h"

#include "FileAdapter.h"

using namespace DiplomBukov;

std::deque<ConnectorModulePtr> BasicLibrary::getConnectorModules() const
{
    std::deque<ConnectorModulePtr> list;
    
    list.push_back(ConnectorModulePtr(new TemplateConnectorModule<BruteConnector>(
        "Коннектор, осуществляющий передачу пакетов всем подключённым процессорам.")));

    list.push_back(ConnectorModulePtr(new TemplateConnectorModule<ProtocolConnector>(
        "Коннектор, осуществляющий передачу пакетов исходя из протокола пакета и "
        "входных протоколов процессоров. Если у пакета нет протокола - он передаётся "
        "всем процессорам. Если у пакета есть протокол - он передаётся только "
        "процессорам с этим же протоколам и процесоорам не имеющим протокола.")));

    return list;
}

std::deque<AdapterModulePtr> BasicLibrary::getAdapterModules() const
{
    std::deque<AdapterModulePtr> list;

    list.push_back(AdapterModulePtr(new TemplateAdapterModule<FileAdapter>(
        "Адаптер, осуществляющий считывание пакетов из файла и запись пакетов файл. "
        "Входной файл должен соответствовать формату PCAP.")));

    return list;
}

std::deque<ProcessorModulePtr> BasicLibrary::getProcessorModules() const
{
    std::deque<ProcessorModulePtr> list;
    return list;
}