#include "QIconTableWidget.h"
#include <QMimeData>
#include <QHeaderView>

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
