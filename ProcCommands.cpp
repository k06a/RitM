#include "ProcCommands.h"

ProcItem::ProcItem(int row, int column, ProcTableWidgetItem * widget)
    : row(row), column(column), widget(widget)
{
}

QString getCommandName(QString str, int size)
{
    QString post = ((size%10==1) && (size%100!=11))
                   ? QObject::tr("а")
                   : QObject::tr("ов");
    return str.arg(size).arg(post);
}

// ----------------------------------------------------------------

RemoveProcCommand::RemoveProcCommand(ProcTableWidget * table,
                                     QList<ProcItem> items)
    : QUndoCommand(getCommandName(QObject::tr("Удаление %1 элемент%2"),items.size()))
    , table(table)
    , items(items)
{
}

void RemoveProcCommand::undo()
{
    table->clearSelection();
    foreach(ProcItem item, items)
    {
        ProcTableWidgetItem * w = new ProcTableWidgetItem(item.widget);
        table->setCellWidget(item.row, item.column, w);
        table->item(item.row, item.column)->setSelected(true);
    }
}

void RemoveProcCommand::redo()
{
    table->clearSelection();
    foreach(ProcItem item, items)
    {
        table->removeCellWidget(item.row, item.column);
        table->item(item.row, item.column)->setSelected(true);
    }
}

// ----------------------------------------------------------------

PutProcCommand::PutProcCommand(ProcTableWidget * table,
                               ProcItem item)
    : QUndoCommand(QObject::tr("Добавление нового элемента"))
    , table(table)
    , item(item)
    , backup(item.row, item.column, NULL)
{
}

void PutProcCommand::undo()
{
    // Reverse to backup
    ProcTableWidgetItem * old_w = backup.widget;
    if (old_w != NULL)
        old_w = new ProcTableWidgetItem(old_w);
    table->setCellWidget(backup.row, backup.column, old_w);

    // Change selection
    table->clearSelection();
    table->item(backup.row, backup.column)->setSelected(true);
}

void PutProcCommand::redo()
{
    // Backup widget
    ProcTableWidgetItem * old_w = (ProcTableWidgetItem*)table->cellWidget(item.row, item.column);
    if (old_w != NULL)
        old_w = new ProcTableWidgetItem(old_w);
    backup.widget = old_w;

    // Put widget
    ProcTableWidgetItem * new_w = item.widget;
    if (new_w != NULL)
        new_w = new ProcTableWidgetItem(new_w);
    table->setCellWidget(item.row, item.column, new_w);

    // Change selection
    table->clearSelection();
    table->item(item.row, item.column)->setSelected(true);
}

// ----------------------------------------------------------------

CopyProcCommand::CopyProcCommand(ProcTableWidget * table,
                                 QList<ProcItem> items,
                                 int touchIndex,
                                 int putRow,
                                 int putColumn)
    : QUndoCommand(getCommandName(QObject::tr("Копирование %1 элемент%2"),items.size()))
    , table(table)
    , items(items)
    , touchIndex(touchIndex)
    , putRow(putRow)
    , putColumn(putColumn)
{
}

void CopyProcCommand::undo()
{
    table->clearSelection();
    foreach(ProcItem item, backup)
    {
        ProcTableWidgetItem * old_w = item.widget;
        if (old_w != NULL)
            old_w = new ProcTableWidgetItem(old_w);
        table->setCellWidget(item.row, item.column, old_w);
        table->item(item.row, item.column)->setSelected(true);
    }
}

void CopyProcCommand::redo()
{
    backup.clear();
    foreach(ProcItem item, items)
    {
        int r = putRow + item.row - items[touchIndex].row;
        int c = putColumn + item.column - items[touchIndex].column;

        ProcTableWidgetItem * old_w = (ProcTableWidgetItem*)table->cellWidget(r, c);
        if (old_w != NULL)
            old_w = new ProcTableWidgetItem(old_w);
        backup.append(ProcItem(r,c,old_w));
    }

    table->clearSelection();
    foreach(ProcItem item, items)
    {
        int r = putRow + item.row - items[touchIndex].row;
        int c = putColumn + item.column - items[touchIndex].column;

        ProcTableWidgetItem * new_w = item.widget;
        if (new_w != NULL)
            new_w = new ProcTableWidgetItem(new_w);
        table->setCellWidget(r, c, new_w);

        table->item(r, c)->setSelected(true);
    }
}

QString CopyProcCommand::toStringForm()
{
    int l = items[0].widget->rect().left();
    int t = items[0].widget->rect().top();

    foreach(ProcItem item, items)
    {
        l = qMin(l, item.widget->rect().left());
        t = qMax(t, item.widget->rect().top());
    }

    QStringList answer;

}

// ----------------------------------------------------------------

MoveProcCommand::MoveProcCommand(ProcTableWidget * table,
                                 QList<ProcItem> items,
                                 int touchIndex,
                                 int putRow,
                                 int putColumn)
    : QUndoCommand(getCommandName(QObject::tr("Перемещение %1 элемент%2"),items.size()))
    , copy(new CopyProcCommand(table,items,touchIndex,putRow,putColumn))
    , remove(new RemoveProcCommand(table,items))
{
}

void MoveProcCommand::undo()
{
    copy->undo();
    remove->undo();
}

void MoveProcCommand::redo()
{
    remove->redo();
    copy->redo();
}

// ----------------------------------------------------------------

