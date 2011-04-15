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

QString getCommandName(QString str, int size);


// ----------------------------------------------------------------

class RemoveProcCommand : public QUndoCommand
{
public:
    RemoveProcCommand(ProcTableWidget * table,
                      QList<ProcItem> items);

    virtual void undo();
    virtual void redo();

private:
    ProcTableWidget * table;
    QList<ProcItem> items;
};

// ----------------------------------------------------------------

class PutProcCommand : public QUndoCommand
{
public:
    PutProcCommand(ProcTableWidget * table,
                   ProcItem item);

    virtual void undo();
    virtual void redo();

private:
    ProcTableWidget * table;
    ProcItem item;

    ProcItem backup;
};

// ----------------------------------------------------------------

class CopyProcCommand : public QUndoCommand
{
public:
    CopyProcCommand(ProcTableWidget * table,
                    QList<ProcItem> items,
                    int touchIndex,
                    int putRow,
                    int putColumn);

    virtual void undo();
    virtual void redo();

    QString toStringForm();

private:
    ProcTableWidget * table;
    QList<ProcItem> items;
    int touchIndex;
    int putRow;
    int putColumn;

    QList<ProcItem> backup;
};

// ----------------------------------------------------------------

class MoveProcCommand : public QUndoCommand
{
public:
    MoveProcCommand(ProcTableWidget * table,
                    QList<ProcItem> items,
                    int touchIndex,
                    int putRow,
                    int putColumn);

    virtual void undo();
    virtual void redo();

private:
    CopyProcCommand * copy;
    RemoveProcCommand * remove;
};

// ----------------------------------------------------------------

#endif // PROCCOMMANDS_H
