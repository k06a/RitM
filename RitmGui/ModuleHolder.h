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

using namespace RitM;

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

    ModuleRecord(const ModuleRecord & mod)
        : lib(mod.lib)
        , name(mod.name)
        , pixmapPath(mod.pixmapPath)
        , adapterModule(mod.adapterModule)
        , connectorModule(mod.connectorModule)
        , processorModule(mod.processorModule)
        , sidesIn(mod.sidesIn)
        , sidesOut(mod.sidesOut)
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
    QString toolTip;

    ModuleRecord module;

    ProcRecord()
        : info("")
    {
    }

    ProcRecord(const ProcRecord & proc)
        : info(proc.info)
        , elementName(proc.elementName)
        , toolTip(proc.toolTip)
        , module(proc.module)
    {
        *this = proc;
    }

    ProcRecord & operator = (const ProcRecord & proc)
    {
        info = proc.info;
        elementName = proc.elementName;
        toolTip = proc.toolTip;
        module = proc.module;

        ProcessorPtr newProc;
        if (proc.adapter != NULL)
        {
            newProc = proc.adapter->CreateCopy();
            adapter = SharedPointerCast<IAdapter>(newProc);
        }
        else if (proc.connector != NULL)
        {
            newProc = proc.connector->CreateCopy();
            connector = SharedPointerCast<IConnector>(newProc);
        }
        else if (proc.processor != NULL)
        {
             newProc = proc.processor->CreateCopy();
             processor = newProc;
        }
        else
            return *this;

        statsProvider = newProc->statsProvider();
        options = newProc->getOptions();

        return *this;
    }

    ProcRecord(ModuleRecord module, int row, int column)
        : info("")
    {
        init(module, row, column);
    }

    void init(ModuleRecord module, int row, int column)
    {
        this->module = module;
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
        } else
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

        // Разбиение строки для ToolTip-а
        QString str = QObject::tr("Строка %1, столбец %2: %3")
            .arg(row+1)
            .arg(column+1)
            .arg(QObject::tr(info));
        while (str.length() > 40)
        {
            int pos = 40;
            while ((str[pos] != ' ') && (pos < str.length())) pos++;
            while (!QChar(str[pos]).isLetter() && (pos < str.length())) pos++;
            toolTip += str.left(pos) + "\n";
            str.remove(0, pos);
        }
        toolTip += str;
    }

    void MoveToCopy()
    {
        if (adapter != NULL)
        {
            adapter = SharedPointerCast<IAdapter>(adapter->CreateCopy());
            statsProvider = adapter->statsProvider();
            options = adapter->getOptions();
        } else
        if (connector != NULL)
        {
            connector = SharedPointerCast<IConnector>(connector->CreateCopy());
            statsProvider = connector->statsProvider();
            options = connector->getOptions();
        } else
        if (processor != NULL)
        {
            processor = processor->CreateCopy();
            statsProvider = processor->statsProvider();
            options = processor->getOptions();
        }
        else
            return;
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
