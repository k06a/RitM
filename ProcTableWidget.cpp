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
    , m_selectedIndexes()
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

// slost

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
            m_selectedIndexes.clear();
            return;
        }

        QWidget * w = indexWidget(indexFromItem(it));
        ProcTableWidgetItem * wit = qobject_cast<ProcTableWidgetItem*>(w);
        if (wit == NULL)
        {
            clearSelection();
            m_selectedIndexes.clear();
            return;
        }

        if (event->modifiers() & Qt::ShiftModifier)
        {
            /*
            int r1 = currentItem()->row();
            int c1 = currentItem()->column();
            int r2 = it->row();
            int c2 = it->column();

            int dr = (r1 < r2) ? 1 : -1;
            int dc = (c1 < c2) ? 1 : -1;

            for (int r = r1; r != r2+dr; r+=dr)
            for (int c = c1; c != c2+dc; c+=dc)
            {
                QTableWidgetItem * i = item(r,c);
                if (i != NULL)
                    i->setSelected(!i->isSelected());
            }
            */
        } else
        if (event->modifiers() & Qt::ControlModifier)
        {
            if (it->isSelected())
            {
                setCurrentItem(it);
                it->setSelected(false);
                m_selectedIndexes.removeOne(indexFromItem(it));
            }
            else
            {
                setCurrentItem(it);
                it->setSelected(true);
                m_selectedIndexes.append(indexFromItem(it));
            }
        }
        else
        {
            if (it->isSelected())
            {
                m_waitForMove = true;
                return;
            }

            clearSelection();
            setCurrentItem(it);
            it->setSelected(true);
            m_selectedIndexes.clear();
            m_selectedIndexes.append(indexFromItem(it));
        }
    }
    //QTableWidget::mousePressEvent(event);
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
    if (m_selectedIndexes.indexOf(indexFromItem(it)) == -1)
    {
        clearSelection();
        m_selectedIndexes.clear();
        return;
    }

    if (m_selectedIndexes.size() > 0)
    {
        QModelIndexList indexes = m_selectedIndexes;

        QRect allRect;
        for (int i = 0; i < indexes.size(); i++)
        {
            int top  = rowViewportPosition(indexes[i].row());
            int left = columnViewportPosition(indexes[i].column());

            QWidget * w = indexWidget(indexes[i]);
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
        for (int i = 0; i < indexes.size(); i++)
        {
            QWidget * w = indexWidget(indexes[i]);
            ProcTableWidgetItem * twi = qobject_cast<ProcTableWidgetItem*>(w);
            int top  = rowViewportPosition(indexes[i].row());
            int left = columnViewportPosition(indexes[i].column());

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
        for (int i = 0; i < indexes.count(); i++)
        {
            QWidget * w = indexWidget(indexes[0]);
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

        m_touchIndex = indexAt(event->pos());
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
        setCurrentItem(it);
        it->setSelected(true);
        m_selectedIndexes.clear();
        m_selectedIndexes.append(indexFromItem(it));
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
    if (event->source() != this)
    {
        const ProcMimeData * mimeData =
            qobject_cast<const ProcMimeData*>(event->mimeData());
        if (mimeData == NULL)
            return;
    }
    event->acceptProposedAction();
}

void ProcTableWidget::dragMoveEvent(QDragMoveEvent * event)
{
    if (event->source() != this)
    {
        const ProcMimeData * mimeData =
            qobject_cast<const ProcMimeData*>(event->mimeData());
        if (mimeData == NULL)
            return;

        //setDropIndicatorShown(true);
        //QTableWidget::dragMoveEvent(event);

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
        const ProcMimeData * mimeData =
            qobject_cast<const ProcMimeData*>(event->mimeData());
        if (mimeData == NULL)
            return;

        if (indexAt(event->pos()) == m_nowTouchIndex)
            return;

        m_nowTouchIndex = indexAt(event->pos());
        int dr = m_nowTouchIndex.row() - m_touchIndex.row();
        int dc = m_nowTouchIndex.column() - m_touchIndex.column();

        clearSelection();
        for (int i = 0; i < m_selectedIndexes.size(); i++)
        {
            int r = m_selectedIndexes[i].row() + dr;
            int c = m_selectedIndexes[i].column() + dc;

            QTableWidgetItem * it = item(r,c);
            if (it == NULL)
            {
                it = new QTableWidgetItem;
                setItem(r, c, it);
            }

            it->setSelected(true);
        }
    }
}

void ProcTableWidget::dragLeaveEvent(QDragLeaveEvent * event)
{
    clearSelection();
    //m_selectedIndexes.clear();
}

void ProcTableWidget::dropEvent(QDropEvent * event)
{
    if (event->source() != this)
    {
        const ProcMimeData * mimeData =
            qobject_cast<const ProcMimeData*>(event->mimeData());
        if (mimeData == NULL)
            return;

        //clearSelection();
        ProcTableWidgetItem * w = new ProcTableWidgetItem();
        ModuleHolder * holder = ModuleHolder::instance();
        ModuleRecord * rec = holder->moduleForName(mimeData->moduleName().toStdString());
        w->setPixmap(rec->pixmap);
        w->setText(rec->fullName().c_str());
        setIndexWidget(indexFromItem(itemAt(event->pos())), w);
        setFocus();

        m_selectedIndexes.clear();
        m_selectedIndexes.append(indexFromItem(itemAt(event->pos())));

        itemAt(event->pos())->setSelected(false);
        setCurrentItem(itemAt(event->pos()));
    }
    else
    {
        m_nowTouchIndex = indexAt(event->pos());
        int dr = m_nowTouchIndex.row() - m_touchIndex.row();
        int dc = m_nowTouchIndex.column() - m_touchIndex.column();

        for (int i = 0; i < m_selectedIndexes.size(); i++)
        {
            QTableWidgetItem * it = itemFromIndex(m_selectedIndexes[i]);

            int r = m_selectedIndexes[i].row() + dr;
            int c = m_selectedIndexes[i].column() + dc;

            QWidget * w = indexWidget(m_selectedIndexes[i]);
            ProcTableWidgetItem * wi = qobject_cast<ProcTableWidgetItem*>(w);

            if (event->dropAction() == Qt::CopyAction)
            {
                setItem(r, c, new QTableWidgetItem());
                setIndexWidget(indexFromItem(item(r,c)), new ProcTableWidgetItem(wi));
                setCurrentItem(item(r,c));
            } else if (event->dropAction() == Qt::MoveAction)
            {
                it = takeItem(m_selectedIndexes[i].row(),
                              m_selectedIndexes[i].column());
                setItem(r, c, it);
                setIndexWidget(indexFromItem(item(r,c)), new ProcTableWidgetItem(wi));
                setIndexWidget(indexFromItem(it), NULL);
                setCurrentItem(item(r,c));
            }

        }
    }

    /*
    if (event->source() != this)
        return;

    TableItem * old = reinterpret_cast<TableItem*>(event->mimeData()->data("").data());
    QModelIndex index = indexAt(event->pos());
    setIndexWidget(index, new TableItem(old));

    event->acceptProposedAction();
    */
}
