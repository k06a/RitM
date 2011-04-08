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
                             QPixmap pixmap,
                             std::string moduleName,
                             std::string libName)
{
    ModuleRecord rec = { libName, moduleName, module, pixmap };
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

ModuleRecord * ModuleHolder::moduleForName(std::string libName,
                                           std::string moduleName)
{
    std::string fullName = libName + '.' + moduleName;
    return moduleForName(fullName);
}

ModuleRecord * ModuleHolder::moduleForName(std::string fullName)
{
    ModuleVector::iterator it = std::find(modules.begin(), modules.end(), fullName);
    if (it == modules.end())
        return NULL;
    return &(*it);
}
