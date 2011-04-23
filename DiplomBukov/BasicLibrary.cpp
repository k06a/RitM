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

std::deque<ConnectorModulePtr> BasicLibrary::getConnectorModules() const
{
    std::deque<ConnectorModulePtr> list;
    
    list.push_back(ConnectorModulePtr(new TemplateConnectorModule<BruteConnector>(
        "���������, �������������� �������� ������� ���� ������������ �����������.")));
    list.push_back(ConnectorModulePtr(new TemplateConnectorModule<ProtocolConnector>(
        "���������, �������������� �������� ������� ������ �� ��������� ������ � "
        "������� ���������� �����������. ���� � ������ ��� ��������� - �� ��������� "
        "���� �����������. ���� � ������ ���� �������� - �� ��������� ������ "
        "����������� � ���� �� ���������� � ����������� �� ������� ���������.")));

    return list;
}

std::deque<AdapterModulePtr> BasicLibrary::getAdapterModules() const
{
    std::deque<AdapterModulePtr> list;

    list.push_back(AdapterModulePtr(new TemplateAdapterModule<FileAdapter>(
        "�������, �������������� ���������� ������� �� ����� � ������ ������� ����. "
        "������� ���� ������ ��������������� ������� PCAP.")));
    list.push_back(AdapterModulePtr(new TemplateAdapterModule<PcapAdapter>(
        "�������, �������������� �������������� ����� �������� � ������� �����������.")));

    return list;
}

std::deque<ProcessorModulePtr> BasicLibrary::getProcessorModules() const
{
    std::deque<ProcessorModulePtr> list;

    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<MacSwitch>(
        "����������, �������������� ���������� ������ ����� ������������ �� MAC-�������.")));

    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<AcceptProcessor>(
        "����������, �������������� ������������� ������.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<RejectProcessor>(
        "����������, �������������� ���������� ������.")));

    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<MacHeaderProcessor>(
        "����������, �������������� ������ ��������� Ethernet II.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<Ipv4HeaderProcessor>(
        "����������, �������������� ������ ��������� IPv4.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpHeaderProcessor>(
        "����������, �������������� ������ ��������� TCP.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<UdpHeaderProcessor>(
        "����������, �������������� ������ ��������� UDP.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<IcmpHeaderProcessor>(
        "����������, �������������� ������ ��������� ICMP.")));

    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<Ipv4Splitter>(
        "����������, �������������� ���������� ������� �� IPv4-�������.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpSplitter>(
        "����������, �������������� ���������� ������� �� TCP-������.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<UdpSplitter>(
        "����������, �������������� ���������� ������� �� UDP-������.")));

    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<TransportPortFilter>(
        "����������, �������������� ���������� ������� �� TCP/UDP-������.")));

    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<Ipv4Defragger>(
        "����������, �������������� (��)������������ IP-�������.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpLayerProcessor>(
        "����������, �������������� ������� ������� ������ �� ������ TCP.")));
    list.push_back(ProcessorModulePtr(new TemplateProcessorModule<TcpOptionsRemover>(
        "����������, �������������� �������� TCP-����� � ������.")));
    
    return list;
}