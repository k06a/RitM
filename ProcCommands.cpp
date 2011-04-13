#include "ProcCommands.h"

ProcItem::ProcItem(int row, int column, ProcTableWidgetItem * widget)
    : row(row), column(column), widget(widget)
{
}

// ----------------------------------------------------------------

PutProcCommand::PutProcCommand(ProcTableWidget * table,
                               QList<ProcItem> items,
                               int touchIndex,
                               int putRow,
                               int putColumn)
    : table(table)
    , items(items)
    , touchIndex(touchIndex)
    , putRow(putRow)
    , putColumn(putColumn)
{
}

void PutProcCommand::undo()
{
    foreach(ProcItem it, backup)
    {
        int r = putRow + it.row - items[touchIndex].row;
        int c = putColumn + it.column - items[touchIndex].column;
        table->setCellWidget(r, c, it.widget);
    }
}

void PutProcCommand::redo()
{
    backup.clear();
    foreach(ProcItem it, items)
    {
        int r = putRow + it.row - items[touchIndex].row;
        int c = putColumn + it.column - items[touchIndex].column;
        backup.append(ProcItem(r,c,it.widget));
        table->setCellWidget(r, c, it.widget);
    }
}

// ----------------------------------------------------------------

RemoveProcCommand::RemoveProcCommand(ProcTableWidget * table,
                                     QList<ProcItem> items)
    : table(table)
    , items(items)
{
    foreach(ProcItem it, items)
        it.widget = new ProcTableWidgetItem(it.widget);
}

void RemoveProcCommand::undo()
{
    foreach(ProcItem it, items)
    {
        table->setCellWidget(it.row, it.column, it.widget);
        it.widget = new ProcTableWidgetItem(it.widget);
    }
}

void RemoveProcCommand::redo()
{
    foreach(ProcItem it, items)
        table->setCellWidget(it.row, it.column, NULL);
}

// ----------------------------------------------------------------

