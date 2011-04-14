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
    : QUndoCommand(getCommandName(items.size()))
    , table(table)
    , items(items)
{
    //for(int i = 0; i < items.size(); i++)
    //{
    //    ProcItem & it = items[i];
    //    it.widget = new ProcTableWidgetItem(it.widget);
    //}
}

void RemoveProcCommand::undo()
{
    for(int i = 0; i < items.size(); i++)
    {
        ProcItem & it = items[i];
        ProcTableWidgetItem * w = new ProcTableWidgetItem(it.widget);
        table->setCellWidget(it.row, it.column, w);
    }
}

void RemoveProcCommand::redo()
{
    for(int i = 0; i < items.size(); i++)
    {
        ProcItem & it = items[i];
        table->removeCellWidget(it.row, it.column);
    }
}

QString RemoveProcCommand::getCommandName(int size)
{
    QString post = ((size%10==1) && (size%100!=11))
                   ? QObject::tr("а")
                   : QObject::tr("ов");
    return QObject::tr("Удаление %1 элемент%2")
           .arg(size)
           .arg(post);
}

// ----------------------------------------------------------------

