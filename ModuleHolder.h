#ifndef MODULEHOLDER_H
#define MODULEHOLDER_H

#include <vector>
#include <string>
#include <algorithm>
#include <QPixmap>

class IModule;

struct ModuleRecord
{
    std::string lib;
    std::string name;
    IModule * module;
    QPixmap pixmap;

    std::string fullName() const
    {
        return (lib + '.' + name);
    }

    bool operator == (const std::string & fullName) const
    {
        return fullName == (lib + '.' + name);
    }
};

class ModuleHolder
{
    typedef std::vector<ModuleRecord> ModuleVector;

    ModuleVector modules;

private:
    static ModuleHolder * m_instance;

public:
    static ModuleHolder * instance();

    void addModule(IModule * module,
                   QPixmap pixmap,
                   std::string moduleName,
                   std::string libName);

    void clear();
    const ModuleVector & moduleList() const;
    ModuleRecord * moduleForName(std::string libName,
                                 std::string moduleName);
    ModuleRecord * moduleForName(std::string fullName);

private:
    ModuleHolder();
};

#endif // MODULEHOLDER_H
