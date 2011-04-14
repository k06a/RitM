#include "ProcTableWidget.h"
#include <QApplication>
#include <QMimeData>
#include <QHeaderView>
#include <QListWidgetItem>
#include <QModelIndexList>
#include <QPainter>
#include <QDebug>
#include "ProcTableWidgetItem.h"
#include "ProcMimeData.h"
#include "ModuleHolder.h"
#include "ProcCommands.h"

ProcTableWidget::ProcTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
}

int ProcTableWidget::baseZoomWidth() const
{
    return m_baseZoomWidth;
}

void ProcTableWidget::setBaseZoomWidth(int value)
{
    m_baseZoomWidth = value;
}

int ProcTableWidget::baseZoomHeight() const
{
    return m_baseZoomHeight;
}

void ProcTableWidget::setBaseZoomHeight(int value)
{
    m_baseZoomHeight = value;
}

float ProcTableWidget::minimumZoom() const
{
    return m_minimumZoom;
}

void ProcTableWidget::setMinimumZoom(float value)
{
    m_minimumZoom = value;
}

float ProcTableWidget::maximumZoom() const
{
    return m_maximumZoom;
}

void ProcTableWidget::setMaximumZoom(float value)
{
    m_maximumZoom = value;
}

float ProcTableWidget::zoomStep() const
{
    return m_zoomStep;
}

void ProcTableWidget::setZoomStep(float value)
{
    m_zoomStep = value;
}

float ProcTableWidget::currentZoom() const
{
    return m_currentZoom;
}

void ProcTableWidget::setCurrentZoom(float value)
{
    m_currentZoom = value;
    if (m_currentZoom > m_maximumZoom)
        m_currentZoom = m_maximumZoom;
    if (m_currentZoom < m_minimumZoom)
        m_currentZoom = m_minimumZoom;
    zoomTo(m_currentZoom);
}

QUndoStack * ProcTableWidget::stack() const
{
    return m_stack;
}

void ProcTableWidget::setStack(QUndoStack * stack)
{
    m_stack = stack;
}

// Cut Copy Paste

QString ProcTableWidget::cut()
{
    QString str = copy();
    deleteSelectedItems();
    return "";
}

QString ProcTableWidget::copy() const
{
    return "";
}

void ProcTableWidget::paste(QString str)
{

}

// private

QRect ProcTableWidget::itemsBoundingRect(QList<QTableWidgetItem*> list) const
{
    QRect answer;
    foreach(QTableWidgetItem * item, list)
    {
        int top  = rowViewportPosition(item->row());
        int left = columnViewportPosition(item->column());

        QWidget * w = cellWidget(item->row(), item->column());
        QRect rect(QPoint(left, top),
                   QPoint(left + w->size().width(),
                          top + w->size().height()));

        if (answer.isEmpty())
        {
            answer = rect;
            continue;
        }


        answer.setLeft  ( qMin( answer.left()  , rect.left()   ) );
        answer.setTop   ( qMin( answer.top()   , rect.top()    ) );
        answer.setRight ( qMax( answer.right() , rect.right()  ) );
        answer.setBottom( qMax( answer.bottom(), rect.bottom() ) );
    }
    return answer;
}

// virtual protected

void ProcTableWidget::wheelEvent(QWheelEvent * event)
{
    if (!(event->modifiers() & Qt::ControlModifier))
    {
        QTableWidget::wheelEvent(event);
        return;
    }

    int delta = event->delta()/120;
    if (delta > 0)
    {
        for (int i = 0; i < delta; i++)
            zoomIn();
    }
    else
    {
        for (int i = 0; i < -delta; i++)
            zoomOut();
    }

    event->accept();
}

void ProcTableWidget::mousePressEvent(QMouseEvent * event)
{
    m_firstTouch = event->pos();
    m_lastTouch.setY(event->pos().y());
    m_lastTouch.setX(event->pos().x());
    QTableWidget::mousePressEvent(event);
}

void ProcTableWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_firstTouch).manhattanLength()
        < QApplication::startDragDistance())
        return;

    QTableWidgetItem * it = itemAt(event->pos());
    if (it == NULL) return;
    if (!itemAt(event->pos())->isSelected())
    {
        clearSelection();
        return;
    }

    // Get non-empty draggable items
    m_dragItems.clear();
    foreach(QModelIndex index, selectedIndexes())
        if (cellWidget(index.row(),index.column()) != 0)
            m_dragItems.append(itemFromIndex(index));
    if (m_dragItems.size() == 0)
        return;

    QRect bigrect = itemsBoundingRect(m_dragItems);

    // Draw drag icons
    QPixmap pixmap(bigrect.size());
    pixmap.fill(QColor(0,0,0,0));
    QPainter p(&pixmap);
    foreach(QTableWidgetItem * item, m_dragItems)
    {
        QWidget * w = cellWidget(item->row(), item->column());
        ProcTableWidgetItem * twi = qobject_cast<ProcTableWidgetItem*>(w);

        int top  = rowViewportPosition(item->row());
        int left = columnViewportPosition(item->column());
        QPoint tl = QPoint(left, top) - bigrect.topLeft();
        QRect rect = QRect(tl, w->size());

        //QPoint tl = w->rect().topLeft() - bigrect.topLeft();
        //QRect rect = QRect(tl, w->size());

        p.drawPixmap(rect, twi->pixmap());
        p.setPen(QColor(0,0,192));
        p.drawRect(rect);
        p.fillRect(rect, QBrush(QColor(0,0,192,128), Qt::Dense4Pattern));
    }
    p.end();

    ProcMimeData * mimeData = new ProcMimeData;
    QDrag * drag = new QDrag(this);
    drag->setHotSpot(m_firstTouch - bigrect.topLeft());
    drag->setPixmap(pixmap);
    drag->setMimeData(mimeData);

    // drag data

    m_touchItem = itemAt(event->pos());
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void ProcTableWidget::mouseReleaseEvent(QMouseEvent * event)
{
    QTableWidgetItem * it = itemAt(event->pos());
    if (it == NULL)
        return;

    if (m_waitForMove)
    {
        clearSelection();
        it->setSelected(true);
        m_waitForMove = false;
    }
}

Qt::DropActions ProcTableWidget::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

void ProcTableWidget::dragEnterEvent(QDragEnterEvent * event)
{
    //if (event->source() != this)
    //{
        const ProcMimeData * mimeData =
            qobject_cast<const ProcMimeData*>(event->mimeData());
        if (mimeData == NULL)
            return;
    //}
    event->acceptProposedAction();
}

void ProcTableWidget::dragMoveEvent(QDragMoveEvent * event)
{
    const ProcMimeData * mimeData =
        qobject_cast<const ProcMimeData*>(event->mimeData());
    if (mimeData == NULL)
        return;
    if ((rowAt(event->pos().y()) == rowAt(m_lastTouch.y())) &&
        (columnAt(event->pos().x()) == columnAt(m_lastTouch.x())) &&
        (m_dragItems.size() != 0))
    {
        return;
    }

    if (event->source() != this)
    {
        clearSelection();

        QTableWidgetItem * it = itemAt(event->pos());
        if (it == NULL)
        {
            it = new QTableWidgetItem();
            setItem(rowAt(event->pos().y()),
                    columnAt(event->pos().x()), it);
        }
        it->setSelected(true);
    }
    else
    {
        clearSelection();

        int dr = rowAt(event->pos().y()) - m_touchItem->row();
        int dc = columnAt(event->pos().x()) - m_touchItem->column();

        for (int i = 0; i < m_dragItems.size(); i++)
        {
            int r = m_dragItems[i]->row() + dr;
            int c = m_dragItems[i]->column() + dc;

            if ((r < 0) || (c < 0))
            {
                clearSelection();
                event->ignore();
                return;
            }

            QTableWidgetItem * it = item(r,c);
            if (it == NULL)
            {
                it = new QTableWidgetItem;
                setItem(r, c, it);
            }

            it->setSelected(true);
        }
    }

    m_lastTouch.setY(rowAt(event->pos().y()));
    m_lastTouch.setX(columnAt(event->pos().x()));
}

void ProcTableWidget::dragLeaveEvent(QDragLeaveEvent * event)
{
    clearSelection();
}

void ProcTableWidget::dropEvent(QDropEvent * event)
{
    const ProcMimeData * mimeData =
        qobject_cast<const ProcMimeData*>(event->mimeData());
    if (mimeData == NULL)
        return;

    if (event->source() != this)
    {
        //clearSelection();
        ModuleHolder * holder = ModuleHolder::instance();
        ModuleRecord * rec = holder->moduleForName(mimeData->moduleName());
        ProcTableWidgetItem * w = new ProcTableWidgetItem();
        w->setPixmap(rec->pixmapPath);
        w->setText(rec->fullName());

        ProcItem proc(rowAt(event->pos().y()),
                      columnAt(event->pos().x()), w);
        m_stack->push(new PutProcCommand(this, proc));
        setFocus();

        //clearSelection();
        //it->setSelected(true);
        m_waitForMove = false;
    }
    else
    {
        clearSelection();

        int dr = rowAt(event->pos().y()) - m_touchItem->row();
        int dc = columnAt(event->pos().x()) - m_touchItem->column();

        QList<ProcTableWidgetItem *> widgetList;
        for (int i = 0; i < m_dragItems.count(); ++i)
        {
            QWidget * w = cellWidget(m_dragItems[i]->row(),
                                     m_dragItems[i]->column());
            ProcTableWidgetItem * wi = qobject_cast<ProcTableWidgetItem*>(w);
            widgetList.append(new ProcTableWidgetItem(wi));

            if (event->dropAction() == Qt::MoveAction)
            {
                removeCellWidget(m_dragItems[i]->row(),
                                 m_dragItems[i]->column());
            }
        }

        for (int i = 0; i < m_dragItems.size(); i++)
        {
            int r = m_dragItems[i]->row() + dr;
            int c = m_dragItems[i]->column() + dc;

            QTableWidgetItem * it = item(r,c);
            setCellWidget(r, c, widgetList[i]);
            it->setSelected(true);
        }
    }
}

// slots

void ProcTableWidget::zoomIn()
{
    setCurrentZoom(m_currentZoom + m_zoomStep);
}

void ProcTableWidget::zoomOut()
{
    setCurrentZoom(m_currentZoom - m_zoomStep);
}

void ProcTableWidget::zoomTo(float value)
{
    //int centerX = size().width() / 2;
    //int centerY = size().height() / 2;
    //QTableWidgetItem * centerItem = itemAt(centerX, centerY);

    int w = m_baseZoomWidth * value;
    int h = m_baseZoomHeight * value;
    setIconSize(QSize(w-4,h-4));
    horizontalHeader()->setDefaultSectionSize(w);
    verticalHeader()->setDefaultSectionSize(h);

    //scrollToItem(centerItem, QAbstractItemView::PositionAtCenter);
}

void ProcTableWidget::deleteSelectedItems()
{
    QList<ProcItem> list;
    foreach(QModelIndex index, selectedIndexes())
    {
        int r = index.row();
        int c = index.column();
        ProcTableWidgetItem * w =
                qobject_cast<ProcTableWidgetItem *>(cellWidget(r,c));
        if (w != 0)
            list.append(ProcItem(r,c,new ProcTableWidgetItem(w)));
    }

    m_stack->push(new RemoveProcCommand(this,list));
}
