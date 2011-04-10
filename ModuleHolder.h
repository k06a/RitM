#ifndef MODULEHOLDER_H
#define MODULEHOLDER_H

#include <vector>
#include <algorithm>
#include <QPixmap>

class IModule;

struct ModuleRecord
{
    QString lib;
    QString name;
    IModule * module;
    QString pixmapPath;

    QString fullName() const
    {
        return (lib + '.' + name);
    }

    bool operator == (QString fullName) const
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
                   QString pixmapPath,
                   QString moduleName,
                   QString libName);

    void clear();
    const ModuleVector & moduleList() const;
    ModuleRecord * moduleForName(QString libName,
                                 QString moduleName);
    ModuleRecord * moduleForName(QString fullName);

private:
    ModuleHolder();
};

#endif // MODULEHOLDER_H
