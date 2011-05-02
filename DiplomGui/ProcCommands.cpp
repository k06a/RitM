#include "ProcCommands.h"

ProcItem::ProcItem(int row, int column, ProcTableWidgetItem * widget)
    : row(row), column(column), widget(widget)
{
}

ProcItem::ProcItem(QString rcw)
{
    QStringList rcwList = rcw.split("|rcw|");
    row = rcwList[0].toInt();
    column = rcwList[1].toInt();
    widget = new ProcTableWidgetItem(rcwList[2], row, column);
}

QString ProcItem::toString()
{
    return QString("%1|rcw|%2|rcw|%3")
           .arg(row)
           .arg(column)
           .arg(widget->toStringForm());
}

bool ProcItem::operator < (const ProcItem & pi) const
{
    if (row != pi.row)
        return (row < pi.row);
    if (column != pi.column)
        return (column < pi.column);
    return false;
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

int RemoveProcCommand::id() const
{
    return 'R' + 'e' + 'm' + 'o' + 'v' + 'e';
}

void RemoveProcCommand::undo()
{
    table->clearSelection();
    foreach(ProcItem item, items)
    {
        ProcTableWidgetItem * w = new ProcTableWidgetItem(item.widget);
        table->setCellWidget(item.row, item.column, w);
        table->item(item.row, item.column)->setSelected(true);
        table->item(item.row, item.column)->setToolTip(item.widget->procRecord().toolTip);
    }
}

void RemoveProcCommand::redo()
{
    table->clearSelection();
    foreach(ProcItem item, items)
    {
        table->removeCellWidget(item.row, item.column);
        table->item(item.row, item.column)->setSelected(true);
        table->item(item.row, item.column)->setToolTip("");
    }
}
/*
bool RemoveProcCommand::mergeWith(const QUndoCommand * command)
{
    const RemoveProcCommand * rem =
            dynamic_cast<const RemoveProcCommand*>(command);
    if (rem == 0)
        return false;

    bool flag = true;
    foreach(ProcItem item, items)
    {
        if (table->cellWidget(item.row,item.column) != NULL)
            flag = false;
    }

    return flag;
}
*/

// ----------------------------------------------------------------

PutProcCommand::PutProcCommand(ProcTableWidget * table,
                               ProcItem item)
    : QUndoCommand(QObject::tr("Добавление нового элемента"))
    , table(table)
    , item(item)
    , backup(item.row, item.column, NULL)
{
}

int PutProcCommand::id() const
{
    return 'P' + 'u' + 't';
}

void PutProcCommand::undo()
{
    // Reverse to backup
    ProcTableWidgetItem * old_w = backup.widget;
    if (old_w != NULL)
        old_w = new ProcTableWidgetItem(old_w);
    table->setCellWidget(backup.row, backup.column, old_w);
    table->item(backup.row, backup.column)->setToolTip(old_w->procRecord().toolTip);

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
    table->item(item.row, item.column)->setToolTip(item.widget->procRecord().toolTip);

    // Change selection
    table->clearSelection();
    table->item(item.row, item.column)->setSelected(true);
}

bool PutProcCommand::mergeWith(const QUndoCommand * command)
{
    const PutProcCommand * put =
            dynamic_cast<const PutProcCommand*>(command);
    if (put == NULL)
        return false;

    return (item.row == put->item.row)
        && (item.column == put->item.column)
        && (item.widget->isEqualProc(put->item.widget));
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
    , insertFromTopLeft(false)
{
}

CopyProcCommand::CopyProcCommand(ProcTableWidget * table,
                                 QString itemList,
                                 int touchIndex,
                                 int putRow,
                                 int putColumn)
    : QUndoCommand(getCommandName(QObject::tr("Копирование %1 элемент%2"),
                                  itemList.split("|proc|").size()))
    , table(table)
    , touchIndex(touchIndex)
    , putRow(putRow)
    , putColumn(putColumn)
    , insertFromTopLeft(true)
{
    QStringList procs = itemList.split("|proc|");
    foreach(QString rcw, procs)
    {
        ProcItem pi(rcw);
        items.append(pi);
    }
}

int CopyProcCommand::id() const
{
    return 'C' + 'o' + 'p' + 'y';
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
        table->item(item.row, item.column)->setToolTip(old_w->procRecord().toolTip);
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
        if (insertFromTopLeft)
        {
            r += items[touchIndex].row;
            c += items[touchIndex].column;
        }

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
        if (insertFromTopLeft)
        {
            r += items[touchIndex].row;
            c += items[touchIndex].column;
        }

        ProcTableWidgetItem * new_w = item.widget;
        if (new_w != NULL)
            new_w = new ProcTableWidgetItem(new_w);
        table->setCellWidget(r, c, new_w);
        table->item(r, c)->setToolTip(new_w->procRecord().toolTip);

        QTableWidgetItem * it = table->item(r,c);
        if (it == NULL)
        {
            it = new QTableWidgetItem;
            table->setItem(r, c, it);
        }
        it->setSelected(true);
    }
}

QString CopyProcCommand::toStringForm()
{
    if (items.size() == 0)
        return "";

    int r1 = items[0].row;
    int c1 = items[0].column;
    foreach(ProcItem item, items)
    {
        r1 = qMin(r1, item.row);
        c1 = qMin(c1, item.column);
    }

    QStringList answer;
    foreach(ProcItem item, items)
    {
        answer << QString("%1|rcw|%2|rcw|%3")
                  .arg(item.row - r1)
                  .arg(item.column - c1)
                  .arg(item.widget->toStringForm());
    }

    return answer.join("|proc|");
}

bool CopyProcCommand::mergeWith(const QUndoCommand * command)
{
    const CopyProcCommand * com =
            dynamic_cast<const CopyProcCommand*>(command);
    if (com == 0)
        return false;

    if (items.size() != com->items.size())
        return false;

    QList<ProcItem> list1;
    QList<ProcItem> list2;
    for(int i = 0; i < items.size(); i++)
    {
        int r1 = putRow + items[i].row - items[touchIndex].row;
        int c1 = putColumn + items[i].column - items[touchIndex].column;
        if (insertFromTopLeft)
        {
            r1 += items[touchIndex].row;
            c1 += items[touchIndex].column;
        }

        int r2 = com->putRow + com->items[i].row - com->items[touchIndex].row;
        int c2 = com->putColumn + com->items[i].column - com->items[touchIndex].column;
        if (com->insertFromTopLeft)
        {
            r2 += com->items[touchIndex].row;
            c2 += com->items[touchIndex].column;
        }

        ProcTableWidgetItem * w1 = (ProcTableWidgetItem*)table->cellWidget(r1,c1);
        ProcTableWidgetItem * w2 = (ProcTableWidgetItem*)table->cellWidget(r2,c2);

        if ((w1 == 0) || (w2 == 0))
            return false;

        list1.append(ProcItem(r1,c1,w1));
        list2.append(ProcItem(r2,c2,w2));
    }

    qSort(list1.begin(), list1.end());
    qSort(list2.begin(), list2.end());
    for (int i = 0; i < list1.size(); i++)
    {
        if ((list1[i].row != list2[i].row) ||
            (list1[i].column != list2[i].column))
        {
            return false;
        }

        if (!list1[i].widget->isEqualProc(list2[i].widget))
            return false;
    }

    return true;
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

int MoveProcCommand::id() const
{
    return 'M' + 'o' + 'v' + 'e';
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

