#include "MitmLibrary.h"

#include "TemplateConnectorModule.h"
#include "TemplateAdapterModule.h"
#include "TemplateProcessorModule.h"

#include "DnsMessageProcessor.h"
#include "HttpDefragProcessor.h"
#include "HttpSwapProcessor.h"
#include "TelnetSwapper.h"

using namespace DiplomBukov;

MitmLibrary::MitmLibrary()
{
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<DnsMessageProcessor>(
        "����������, �������������� ������ ������ DNS.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<HttpDefragProcessor>(
        "����������, �������������� ������ ��������� HTTP.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<HttpSwapProcessor>(
        "����������, �������������� ������ ������ HTTP.")));
    list_proc.push_back(ProcessorModulePtr(new TemplateProcessorModule<TelnetSwapper>(
        "����������, �������������� ������ ������ Telnet.")));
}

int MitmLibrary::getConnectorModules_size() const
{
    return list_con.size();
}

int MitmLibrary::getAdapterModules_size() const
{
    return list_ad.size();
}

int MitmLibrary::getProcessorModules_size() const
{
    return list_proc.size();
}

ConnectorModulePtr MitmLibrary::getConnectorModules_item(int i) const
{
    return list_con[i];
}

AdapterModulePtr MitmLibrary::getAdapterModules_item(int i) const
{
    return list_ad[i];
}

ProcessorModulePtr MitmLibrary::getProcessorModules_item(int i) const
{
    return list_proc[i];
}