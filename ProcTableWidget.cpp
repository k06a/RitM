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
    if (event->button() == Qt::LeftButton)
    {
        m_dragStartPosition = event->pos();
        QTableWidgetItem * it = itemAt(event->pos());
        if (it == NULL)
        {
            clearSelection();
            m_selectedItems.clear();
            return;
        }

        QWidget * w = cellWidget(it->row(), it->column());
        ProcTableWidgetItem * wit = qobject_cast<ProcTableWidgetItem*>(w);
        if (wit == NULL)
        {
            clearSelection();
            m_selectedItems.clear();
            return;
        }

        if (event->modifiers() & Qt::ControlModifier)
        {
            if (it->isSelected())
            {
                it->setSelected(false);
                m_selectedItems.removeOne(it);
            }
            else
            {
                it->setSelected(true);
                m_selectedItems.append(it);
            }
        }
        else
        {
            if (it->isSelected())
            {
                m_lastTouchCoord.setY(rowAt(event->pos().y()));
                m_lastTouchCoord.setX(columnAt(event->pos().x()));
                m_waitForMove = true;
                return;
            }

            clearSelection();
            it->setSelected(true);
            m_selectedItems.clear();
            m_selectedItems.append(it);
        }
    }
}

void ProcTableWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_dragStartPosition).manhattanLength()
        < QApplication::startDragDistance())
        return;

    QTableWidgetItem * it = itemAt(event->pos());
    if (it == NULL)
        return;
    if (m_selectedItems.indexOf(it) == -1)
    {
        clearSelection();
        return;
    }

    if (m_selectedItems.size() > 0)
    {
        QRect allRect;
        for (int i = 0; i < m_selectedItems.size(); i++)
        {
            int top  = rowViewportPosition(m_selectedItems[i]->row());
            int left = columnViewportPosition(m_selectedItems[i]->column());

            QWidget * w = cellWidget(m_selectedItems[i]->row(), m_selectedItems[i]->column());
            QPoint tl = QPoint(left, top);
            QPoint br = QPoint(tl.x() + w->size().width(),
                               tl.y() + w->size().height());

            if (allRect.size().isEmpty())
            {
                allRect.setTopLeft(tl);
                allRect.setBottomRight(br);
                continue;
            }

            if (allRect.left() > tl.x())
                allRect.setLeft(tl.x());
            if (allRect.right() < br.x())
                allRect.setRight(br.x());
            if (allRect.top() > tl.y())
                allRect.setTop(tl.y());
            if (allRect.bottom() < br.y())
                allRect.setBottom(br.y());
        }

        // Draw drag icons
        QPixmap pixmap(allRect.size());
        pixmap.fill(QColor(0,0,0,0));
        QPainter p(&pixmap);
        for (int i = 0; i < m_selectedItems.size(); i++)
        {
            QWidget * w = cellWidget(m_selectedItems[i]->row(), m_selectedItems[i]->column());
            ProcTableWidgetItem * twi = qobject_cast<ProcTableWidgetItem*>(w);
            int top  = rowViewportPosition(m_selectedItems[i]->row());
            int left = columnViewportPosition(m_selectedItems[i]->column());

            QPoint tl = QPoint(left, top) - allRect.topLeft();

            QRect r = QRect(tl, w->size());
            p.drawPixmap(r, twi->pixmap());

            p.setPen(QColor(0,0,192));
            p.drawRect(r);
            p.fillRect(r, QBrush(QColor(0,0,192,128), Qt::Dense4Pattern));
        }
        p.end();

        // Make name list
        QString itemNames;
        for (int i = 0; i < m_selectedItems.count(); i++)
        {
            QWidget * w = cellWidget(m_selectedItems[i]->row(), m_selectedItems[i]->column());
            ProcTableWidgetItem * twi = qobject_cast<ProcTableWidgetItem*>(w);
            if (i != 0)
                itemNames += "$";
            itemNames += twi->text();
        }

        ProcMimeData * mimeData = new ProcMimeData;
        mimeData->setModuleName(itemNames);
        QDrag * drag = new QDrag(this);
        drag->setHotSpot(m_dragStartPosition - allRect.topLeft());
        drag->setPixmap(pixmap);
        drag->setMimeData(mimeData);

        // drag data

        m_touchItem = itemAt(event->pos());
        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
    }

    // ...
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
        m_selectedItems.clear();
        m_selectedItems.append(it);
        m_waitForMove = false;
    }
    //m_selectedIndexes.clear();
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
    if ((rowAt(event->pos().y()) == m_lastTouchCoord.y()) &&
        (columnAt(event->pos().x()) == m_lastTouchCoord.x()) &&
        (selectedIndexes().size() != 0))
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

        for (int i = 0; i < m_selectedItems.size(); i++)
        {
            int r = m_selectedItems[i]->row() + dr;
            int c = m_selectedItems[i]->column() + dc;

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

    m_lastTouchCoord.setY(rowAt(event->pos().y()));
    m_lastTouchCoord.setX(columnAt(event->pos().x()));
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
        QTableWidgetItem * it = itemAt(event->pos());

        //clearSelection();
        ModuleHolder * holder = ModuleHolder::instance();
        ModuleRecord * rec = holder->moduleForName(mimeData->moduleName().toStdString());
        ProcTableWidgetItem * w = new ProcTableWidgetItem();
        w->setPixmap(rec->pixmap);
        w->setText(rec->fullName().c_str());
        setCellWidget(it->row(), it->column(), w);
        setFocus();

        clearSelection();
        it->setSelected(true);
        m_selectedItems.clear();
        m_selectedItems.append(it);
        m_waitForMove = false;
    }
    else
    {
        clearSelection();

        int dr = rowAt(event->pos().y()) - m_touchItem->row();
        int dc = columnAt(event->pos().x()) - m_touchItem->column();

        QList<ProcTableWidgetItem *> widgetList;
        for (int i = 0; i < m_selectedItems.count(); ++i)
        {
            QWidget * w = cellWidget(m_selectedItems[i]->row(),
                                     m_selectedItems[i]->column());
            ProcTableWidgetItem * wi = qobject_cast<ProcTableWidgetItem*>(w);
            widgetList.append(new ProcTableWidgetItem(wi));

            if (event->dropAction() == Qt::MoveAction)
            {
                removeCellWidget(m_selectedItems[i]->row(),
                                 m_selectedItems[i]->column());
            }
        }

        QList<QTableWidgetItem*> newListOfSelected;
        for (int i = 0; i < m_selectedItems.size(); i++)
        {
            int r = m_selectedItems[i]->row() + dr;
            int c = m_selectedItems[i]->column() + dc;

            QTableWidgetItem * it = item(r,c);
            setCellWidget(r, c, widgetList[i]);
            it->setSelected(true);
            newListOfSelected.append(it);
        }

        m_selectedItems = newListOfSelected;
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
    for(int i = 0; i < m_selectedItems.size(); i++)
    {
        removeCellWidget(m_selectedItems[i]->row(),
                         m_selectedItems[i]->column());
    }
}
