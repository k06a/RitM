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
    ProcItem(QString rcw);

    QString toString();

    bool operator < (const ProcItem & pi) const;
};

QString getCommandName(QString str, int size);


// ----------------------------------------------------------------

class RemoveProcCommand : public QUndoCommand
{
public:
    RemoveProcCommand(ProcTableWidget * table,
                      QList<ProcItem> items);

    virtual int	id () const;
    virtual void undo();
    virtual void redo();
    //virtual bool mergeWith(const QUndoCommand * command);

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

    virtual int	id () const;
    virtual void undo();
    virtual void redo();
    virtual bool mergeWith(const QUndoCommand * command);

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

    CopyProcCommand(ProcTableWidget * table,
                    QString itemList,
                    int touchIndex,
                    int putRow,
                    int putColumn);

    virtual int	id () const;
    virtual void undo();
    virtual void redo();
    bool mergeWith(const QUndoCommand * command);

    QString toStringForm();

private:
    ProcTableWidget * table;
    QList<ProcItem> items;
    int touchIndex;
    int putRow;
    int putColumn;
    bool insertFromTopLeft;

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

    virtual int	id () const;
    virtual void undo();
    virtual void redo();

private:
    CopyProcCommand * copy;
    RemoveProcCommand * remove;
};

// ----------------------------------------------------------------

#endif // PROCCOMMANDS_H
