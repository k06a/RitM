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

    return list;
}

std::deque<ProcessorModulePtr> BasicLibrary::getProcessorModules() const
{
    std::deque<ProcessorModulePtr> list;
    return list;
}