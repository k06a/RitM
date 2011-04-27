#ifndef MODULEHOLDER_H
#define MODULEHOLDER_H

#include "CommonInclude.h"
#include <QString>
#include <QList>

using namespace DiplomBukov;

struct Module
{
    AdapterModulePtr   adapterModule;
    ConnectorModulePtr connectorModule;
    ProcessorModulePtr processorModule;
    Module()
        : adapterModule()
        , connectorModule()
        , processorModule()
    {
    }
};

struct ModuleRecord
{
    QString lib;
    QString name;
    QString pixmapPath;
    Module module;

    ModuleRecord(QString lib = "",
                 QString name = "",
                 QString pixmapPath = "")
        : lib(lib)
        , name(name)
        , pixmapPath(pixmapPath)
    {
    }

    QString fullName() const
    {
        return (lib + '.' + name);
    }

    bool operator == (const QString & fullName) const
    {
        return fullName == (lib + '.' + name);
    }
};

class QListWidget;

class ModuleHolder
{
    typedef QList<ModuleRecord> ModuleVector;

    ModuleVector modules;
    QListWidget * list;

private:
    static ModuleHolder * m_instance;

public:
    static ModuleHolder * instance(QListWidget * list = 0);

    void addModule(QString libName,
                   QString moduleName,
                   QString pixmapPath);

    void clear();
    const ModuleVector & moduleList() const;
    ModuleRecord * moduleForName(QString libName,
                                 QString moduleName);
    ModuleRecord * moduleForName(QString fullName);

    void LoadLibrary(QString dllName);

private:
    ModuleHolder(QListWidget * list = 0);
};

#endif // MODULEHOLDER_H
