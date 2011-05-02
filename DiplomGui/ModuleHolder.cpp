#include "ModuleHolder.h"
#include "ILibrary.h"
#include "IAdapterModule.h"
#include "IConnectorModule.h"
#include "IProcessorModule.h"
#include <QDir>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLibrary>
#include <QCoreApplication>

using namespace DiplomBukov;

ModuleHolder * ModuleHolder::m_instance = NULL;

ModuleHolder::ModuleHolder(QListWidget * list)
    : list(list)
{
    QDir dir(QCoreApplication::applicationDirPath());// + "/plugins");
    QFileInfoList dlls = dir.entryInfoList(QStringList() << "*.dll");
    foreach(QFileInfo dll, dlls)
        LoadLibrary(dll.absoluteFilePath());
}

void ModuleHolder::LoadLibrary(QString dllName)
{
    QLibrary lib(dllName);
    TCreateLibrary libCreate = (TCreateLibrary)lib.resolve("createLibrary");
    TDeleteLibrary libDelete = (TDeleteLibrary)lib.resolve("deleteLibrary");
    if ((libCreate == NULL) || (libDelete == NULL))
    {
        QString p = lib.errorString();
        return;
    }
    ILibrary * library(libCreate());

    ModuleVector newList;
    QString moduleName = QFileInfo(dllName).fileName();
    moduleName.chop(4);

    for (int i = 0; i < library->getAdapterModules_size(); i++)
    {
        ModuleRecord mr(Direction::None, Direction::Right);
        mr.lib = moduleName;
        mr.adapterModule = library->getAdapterModules_item(i);
        mr.name = mr.adapterModule->name();
        mr.pixmapPath = QObject::tr(":/images/adapter.svg");
        newList.append(mr);
    }

    for (int i = 0; i < library->getConnectorModules_size(); i++)
    {
        ModuleRecord mr(Direction::Left, Direction::Right);
        mr.lib = moduleName;
        mr.connectorModule = library->getConnectorModules_item(i);
        mr.name = mr.connectorModule->name();
        mr.pixmapPath = QObject::tr(":/images/connector.svg");
        newList.append(mr);
    }

    for (int i = 0; i < library->getProcessorModules_size(); i++)
    {
        ModuleRecord mr(Direction::Left, Direction::Right);
        mr.lib = moduleName;
        mr.processorModule = library->getProcessorModules_item(i);
        mr.name = mr.processorModule->name();
        mr.pixmapPath = QObject::tr(":/images/processor.svg");
        if (mr.processorModule->createProcessor()->isFinal())
        {
            mr.pixmapPath = QObject::tr(":/images/processor_end.svg");
            mr.sidesOut.clear();
        }
        newList.append(mr);
    }

    foreach(ModuleRecord mr, newList)
    {
        QListWidgetItem * it = new QListWidgetItem();
        it->setText(mr.fullName());
        it->setToolTip(mr.fullName());
        it->setIcon(QIcon(mr.pixmapPath));
        list->addItem(it);
    }

    modules.append(newList);
    libDelete(library);
}

ModuleHolder * ModuleHolder::instance(QListWidget * list)
{
    if (m_instance == NULL)
        m_instance = new ModuleHolder(list);
    return m_instance;
}

void ModuleHolder::addModule(int directionIn,
                             int directionOut,
                             QString libName,
                             QString moduleName,
                             QString pixmapPath)
{
    ModuleRecord rec(directionIn, directionOut, libName, moduleName, pixmapPath);
    modules.push_back(rec);
}

void ModuleHolder::clear()
{
    modules.clear();
}

const ModuleHolder::ModuleVector & ModuleHolder::moduleList() const
{
    return modules;
}

ModuleRecord * ModuleHolder::moduleForName(QString libName,
                                           QString moduleName)
{
    QString fullName = libName + '.' + moduleName;
    return moduleForName(fullName);
}

ModuleRecord * ModuleHolder::moduleForName(QString fullName)
{
    QList<ModuleRecord>::iterator it =
            qFind(modules.begin(), modules.end() ,fullName);
    if (it == modules.end())
        return NULL;
    return &(*it);
}
