#include "QIconTableWidget.h"
#include <QApplication>
#include <QMimeData>
#include <QHeaderView>
#include "TableItem.h"

QIconTableWidget::QIconTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
}

int QIconTableWidget::baseZoomWidth() const
{
    return m_baseZoomWidth;
}

void QIconTableWidget::setBaseZoomWidth(int value)
{
    m_baseZoomWidth = value;
}

int QIconTableWidget::baseZoomHeight() const
{
    return m_baseZoomHeight;
}

void QIconTableWidget::setBaseZoomHeight(int value)
{
    m_baseZoomHeight = value;
}

float QIconTableWidget::minimumZoom() const
{
    return m_minimumZoom;
}

void QIconTableWidget::setMinimumZoom(float value)
{
    m_minimumZoom = value;
}

float QIconTableWidget::maximumZoom() const
{
    return m_maximumZoom;
}

void QIconTableWidget::setMaximumZoom(float value)
{
    m_maximumZoom = value;
}

float QIconTableWidget::zoomStep() const
{
    return m_zoomStep;
}

void QIconTableWidget::setZoomStep(float value)
{
    m_zoomStep = value;
}

float QIconTableWidget::currentZoom() const
{
    return m_currentZoom;
}

void QIconTableWidget::setCurrentZoom(float value)
{
    m_currentZoom = value;
    if (m_currentZoom > m_maximumZoom)
        m_currentZoom = m_maximumZoom;
    if (m_currentZoom < m_minimumZoom)
        m_currentZoom = m_minimumZoom;
    zoomTo(m_currentZoom);
}

// slost

void QIconTableWidget::zoomIn()
{
    m_currentZoom += m_zoomStep;
    if (m_currentZoom > m_maximumZoom)
        m_currentZoom = m_maximumZoom;
    if (m_currentZoom < m_minimumZoom)
        m_currentZoom = m_minimumZoom;
    zoomTo(m_currentZoom);
}

void QIconTableWidget::zoomOut()
{
    m_currentZoom -= m_zoomStep;
    if (m_currentZoom > m_maximumZoom)
        m_currentZoom = m_maximumZoom;
    if (m_currentZoom < m_minimumZoom)
        m_currentZoom = m_minimumZoom;
    zoomTo(m_currentZoom);
}

void QIconTableWidget::zoomTo(float value)
{
    int w = m_baseZoomWidth * value;
    int h = m_baseZoomHeight * value;
    setIconSize(QSize(w-4,h-4));
    horizontalHeader()->setDefaultSectionSize(w);
    verticalHeader()->setDefaultSectionSize(h);
}

// virtual protected

void QIconTableWidget::wheelEvent(QWheelEvent * event)
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

void QIconTableWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragStartPosition = event->pos();
        itemAt(event->pos())->setSelected(true);
    }
    //QTableWidget::mousePressEvent(event);
}

void QIconTableWidget::mouseMoveEvent(QMouseEvent * event)
{
    /*
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_dragStartPosition).manhattanLength()
            < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setData(mimeType, data);
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
    // ...
    */
}

void QIconTableWidget::dropEvent(QDropEvent * event)
{
    /*
    if (event->source() != this)
        return;

    TableItem * old = reinterpret_cast<TableItem*>(event->mimeData()->data("").data());
    QModelIndex index = indexAt(event->pos());
    setIndexWidget(index, new TableItem(old));

    event->acceptProposedAction();
    */
}
