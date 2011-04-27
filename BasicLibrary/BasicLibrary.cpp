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
        "BruteConnector", "���������, �������������� �������� ������� ���� ������������ �����������.")));
    list_con.push_back(ConnectorModulePtr(new TemplateConnectorModule<ProtocolConnector>(
        "ProtocolConnector", "���������, �������������� �������� ������� ������ �� ��������� ������ � "
        "������� ���������� �����������. ���� � ������ ��� ��������� - �� ��������� "
        "���� �����������. ���� � ������ ���� �������� - �� ��������� ������ "
        "����������� � ���� �� ���������� � ����������� �� ������� ���������.")));

    // ----------------------------------------------------------------

    list_ad.push_back(AdapterModulePtr(new TemplateAdapterModule<FileAdapter>(
        "FileAdapter", "�������, �������������� ���������� ������� �� ����� � ������ ������� ����. "
        "������� ���� ������ ��������������� ������� PCAP.")));
    list_ad.push_back(AdapterModulePtr(new TemplateAdapterModule<PcapAdapter>(
        "PcapAdapter", "�������, �������������� �������������� ����� �������� � ������� �����������.")));

    // ----------------------------------------------------------------

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<MacSwitch>(
        "MacSwitch", "����������, �������������� ���������� ������ ����� ������������ �� MAC-�������.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<AcceptProcessor>(
        "AcceptProcessor", "����������, �������������� ������������� ������.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<RejectProcessor>(
        "RejectProcessor", "����������, �������������� ���������� ������.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<MacHeaderProcessor>(
        "MacHeaderProcessor", "����������, �������������� ������ ��������� Ethernet II.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<Ipv4HeaderProcessor>(
        "Ipv4HeaderProcessor", "����������, �������������� ������ ��������� IPv4.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpHeaderProcessor>(
        "TcpHeaderProcessor", "����������, �������������� ������ ��������� TCP.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<UdpHeaderProcessor>(
        "UdpHeaderProcessor", "����������, �������������� ������ ��������� UDP.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<IcmpHeaderProcessor>(
        "IcmpHeaderProcessor", "����������, �������������� ������ ��������� ICMP.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<Ipv4Splitter>(
        "Ipv4Splitter", "����������, �������������� ���������� ������� �� IPv4-�������.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpSplitter>(
        "TcpSplitter", "����������, �������������� ���������� ������� �� TCP-������.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<UdpSplitter>(
        "UdpSplitter", "����������, �������������� ���������� ������� �� UDP-������.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TransportPortFilter>(
        "TransportPortFilter", "����������, �������������� ���������� ������� �� TCP/UDP-������.")));

    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<Ipv4Defragger>(
        "Ipv4Defragger", "����������, �������������� (��)������������ IP-�������.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpLayerProcessor>(
        "TcpLayerProcessor", "����������, �������������� ������� ������� ������ �� ������ TCP.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpOptionsRemover>(
        "TcpOptionsRemover", "����������, �������������� �������� TCP-����� � ������.")));
    
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