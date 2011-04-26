#include "BasicLibrary.h"

#include "TemplateConnectorModule.h"
#include "TemplateAdapterModule.h"
#include "TemplateProcessorModule.h"

#include "BruteConnector.h"
#include "ProtocolConnector.h"

#include "FileAdapter.h"
#include "PcapAdapter.h"

#include "MacSwitch.h"

#include "AcceptProcessor.h"
#include "RejectProcessor.h"

#include "MacHeaderProcessor.h"
#include "Ipv4HeaderProcessor.h"
#include "TcpHeaderProcessor.h"
#include "UdpHeaderProcessor.h"
#include "IcmpHeaderProcessor.h"

#include "Ipv4Splitter.h"
#include "TcpSplitter.h"
#include "UdpSplitter.h"

#include "TransportPortFilter.h"

#include "Ipv4Defragger.h"
#include "TcpLayerProcessor.h"
#include "TcpOptionsRemover.h"

using namespace DiplomBukov;

BasicLibrary::BasicLibrary()
{
    // ----------------------------------------------------------------

    list_con.push_back(ConnectorModulePtr(new TemplateConnectorModule<BruteConnector>(
        " оннектор, осуществл€ющий передачу пакетов всем подключЄнным процессорам.")));
    list_con.push_back(ConnectorModulePtr(new TemplateConnectorModule<ProtocolConnector>(
        " оннектор, осуществл€ющий передачу пакетов исход€ из протокола пакета и "
        "входных протоколов процессоров. ≈сли у пакета нет протокола - он передаЄтс€ "
        "всем процессорам. ≈сли у пакета есть протокол - он передаЄтс€ только "
        "процессорам с этим же протоколам и процесоорам не имеющим протокола.")));

    // ----------------------------------------------------------------

    list_ad.push_back(AdapterModulePtr(new TemplateAdapterModule<FileAdapter>(
        "јдаптер, осуществл€ющий считывание пакетов из файла и запись пакетов файл. "
        "¬ходной файл должен соответствовать формату PCAP.")));
    list_ad.push_back(AdapterModulePtr(new TemplateAdapterModule<PcapAdapter>(
        "јдаптер, осуществл€ющий информационный обмен пакетами с сетевым интерфейсом.")));

    // ----------------------------------------------------------------

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<MacSwitch>(
        "ќбработчик, осуществл€ющий коммутацию кадров между интерфейсами по MAC-адресам.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<AcceptProcessor>(
        "ќбработчик, осуществл€ющий проталкивание пакета.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<RejectProcessor>(
        "ќбработчик, осуществл€ющий отклонение пакета.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<MacHeaderProcessor>(
        "ќбработчик, осуществл€ющий разбор заголовка Ethernet II.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<Ipv4HeaderProcessor>(
        "ќбработчик, осуществл€ющий разбор заголовка IPv4.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpHeaderProcessor>(
        "ќбработчик, осуществл€ющий разбор заголовка TCP.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<UdpHeaderProcessor>(
        "ќбработчик, осуществл€ющий разбор заголовка UDP.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<IcmpHeaderProcessor>(
        "ќбработчик, осуществл€ющий разбор заголовка ICMP.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<Ipv4Splitter>(
        "ќбработчик, осуществл€ющий разделение потоков по IPv4-адресам.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpSplitter>(
        "ќбработчик, осуществл€ющий разделение потоков по TCP-портам.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<UdpSplitter>(
        "ќбработчик, осуществл€ющий разделение потоков по UDP-портам.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TransportPortFilter>(
        "ќбработчик, осуществл€ющий фильтрацию потоков по TCP/UDP-портам.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<Ipv4Defragger>(
        "ќбработчик, осуществл€ющий (де)фрагментацию IP-пакетов.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpLayerProcessor>(
        "ќбработчик, осуществл€ющий подмену потоков данных на уровне TCP.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpOptionsRemover>(
        "ќбработчик, осуществл€ющий стирание TCP-опций в пакете.")));
    
    // ----------------------------------------------------------------
}

int BasicLibrary::getConnectorModules_size() const
{
    return list_con.size();
}

int BasicLibrary::getAdapterModules_size() const
{
    return list_ad.size();
}

int BasicLibrary::getProcessorModules_size() const
{
    return list_proc.size();
}

ConnectorModulePtr BasicLibrary::getConnectorModules_item(int i) const
{
    return list_con[i];
}

AdapterModulePtr BasicLibrary::getAdapterModules_item(int i) const
{
    return list_ad[i];
}

ProcessorModulePtr BasicLibrary::getProcessorModules_item(int i) const
{
    return list_proc[i];
}