#ifndef MODULEHOLDER_H
#define MODULEHOLDER_H

#include "CommonInclude.h"
#include "IAdapterModule.h"
#include "IConnectorModule.h"
#include "IProcessorModule.h"
#include "IAdapter.h"
#include "IConnector.h"
#include "IProcessor.h"
#include <QString>
#include <QList>
#include <QPair>

using namespace DiplomBukov;

struct Direction
{
    enum DirectionEnum
    {
        None   = 0,
        Left   = 1,
        Right  = 2,
        Top    = 4,
        Bottom = 8,

        LeftRight      = Left   | Right,
        TopBottom      = Top    | Bottom,

        LeftTop        = Left   | Top,
        LeftBottom     = Left   | Bottom,
        TopRight       = Top    | Right,
        BottomRight    = Bottom | Right,

        LeftTopBottom  = Left   | Top    | Bottom,
        TopBottomRight = Top    | Bottom | Right
    };

    static int inverse(int dir)
    {
        int ans = 0;
        if (dir & Left)   ans |= Right;
        if (dir & Right)  ans |= Left;
        if (dir & Top)    ans |= Bottom;
        if (dir & Bottom) ans |= Top;
        return ans;
    }

    static QList<QPair<int,int> > toShift(int dir)
    {
        QList<QPair<int,int> > list;
        if (dir & Left)   list.append(toShiftBasic(Left));
        if (dir & Right)  list.append(toShiftBasic(Right));
        if (dir & Top)    list.append(toShiftBasic(Top));
        if (dir & Bottom) list.append(toShiftBasic(Bottom));
        return list;
    }

    static QPair<int,int> toShiftBasic(DirectionEnum dir)
    {
        switch (dir)
        {
            case Left:   return qMakePair(0,-1);
            case Right:  return qMakePair(0,+1);
            case Top:    return qMakePair(-1,0);
            case Bottom: return qMakePair(+1,0);
        }
        return qMakePair(0,0);
    }
};

struct ModuleRecord
{
    QString lib;
    QString name;
    QString pixmapPath;
    
    AdapterModulePtr   adapterModule;
    ConnectorModulePtr connectorModule;
    ProcessorModulePtr processorModule;

    QList<QPair<int,int> > sidesIn;
    QList<QPair<int,int> > sidesOut;

    ModuleRecord()
    {
    }

    ModuleRecord(int directionIn,
                 int directionOut,
                 QString lib = "",
                 QString name = "",
                 QString pixmapPath = "")
        : lib(lib)
        , name(name)
        , pixmapPath(pixmapPath)
    {
        sidesIn.append(Direction::toShift(directionIn));
        sidesOut.append(Direction::toShift(directionOut));
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

struct ProcRecord
{
    AdapterPtr   adapter;
    ConnectorPtr connector;
    ProcessorPtr processor;
    StatsProviderPtr statsProvider;
    OptionPtr options;
    const char * info;
    QString elementName;

    ModuleRecord module;

    ProcRecord()
        : info("")
    {
    }

    ProcRecord(ModuleRecord module)
        : info(""), module(module)
    {
        if (module.adapterModule != NULL)
        {
            adapter = module.adapterModule->createAdapter();
            statsProvider = adapter->statsProvider();
            options = adapter->getOptions();
            info = module.adapterModule->info();
            elementName = QObject::tr("адапетер");
        } else
        if (module.connectorModule != NULL)
        {
            connector = module.connectorModule->createConnector();
            statsProvider = connector->statsProvider();
            options = connector->getOptions();
            info = module.connectorModule->info();
            elementName = QObject::tr("коннектор");
        }
        if (module.processorModule != NULL)
        {
            processor = module.processorModule->createProcessor();
            statsProvider = processor->statsProvider();
            options = processor->getOptions();
            info = module.processorModule->info();
            elementName = QObject::tr("процессор");
        } else
        {
            info = "Трубы предназначены для создания связей между обработчиками.";
            elementName = QObject::tr("труба");
        }
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

    void addModule(int directionIn,
                   int directionOut,
                   QString libName,
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
