#ifndef PROCCOMMANDS_H
#define PROCCOMMANDS_H

#include <QList>
#include <QUndoCommand>
#include "ProcTableWidget.h"
#include "ProcTableWidgetItem.h"

struct ProcItem
{
    int row;
    int column;
    ProcTableWidgetItem * widget;

    ProcItem(int row, int column, ProcTableWidgetItem * widget);
};

// ----------------------------------------------------------------

class PutProcCommand : public QUndoCommand
{
public:
    PutProcCommand(ProcTableWidget * table,
                   QList<ProcItem> items,
                   int touchIndex,
                   int putRow,
                   int putColumn);

    virtual void undo();
    virtual void redo();

private:
    ProcTableWidget * table;
    QList<ProcItem> items;
    int touchIndex;
    int putRow;
    int putColumn;

    QList<ProcItem> backup;
};

// ----------------------------------------------------------------

class RemoveProcCommand : public QUndoCommand
{
public:
    RemoveProcCommand(ProcTableWidget * table,
                      QList<ProcItem> items);

    virtual void undo();
    virtual void redo();

    static QString getCommandName(int size);

private:
    ProcTableWidget * table;
    QList<ProcItem> items;
};

// ----------------------------------------------------------------

#endif // PROCCOMMANDS_H
