#include "ModuleHolder.h"

ModuleHolder * ModuleHolder::m_instance = NULL;

ModuleHolder::ModuleHolder()
{
}

ModuleHolder * ModuleHolder::instance()
{
    if (m_instance == NULL)
        m_instance = new ModuleHolder();
    return m_instance;
}

void ModuleHolder::addModule(IModule * module,
                             QString pixmapPath,
                             QString moduleName,
                             QString libName)
{
    ModuleRecord rec = { libName, moduleName, module, pixmapPath };
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
