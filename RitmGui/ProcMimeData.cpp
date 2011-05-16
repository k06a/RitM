#include "ProcMimeData.h"

ProcMimeData::ProcMimeData()
    : QMimeData()
{
}

QString ProcMimeData::moduleName() const
{
    return m_moduleName;
}

void ProcMimeData::setModuleName(QString moduleName)
{
    m_moduleName = moduleName;
}

ProcTableWidget * ProcMimeData::tableWidget() const
{
    return m_tableWidget;
}

void ProcMimeData::setTableWidget(ProcTableWidget * tableWidget)
{
    m_tableWidget = tableWidget;
}

ModuleRecord ProcMimeData::module() const
{
    return m_module;
}

void ProcMimeData::setModule(ModuleRecord module)
{
    m_module = module;
}
