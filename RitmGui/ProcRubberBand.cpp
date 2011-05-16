#include <QPainter>
#include "ProcRubberBand.h"

ProcRubberBand::ProcRubberBand(QRubberBand::Shape shape, QWidget * parent)
    : QRubberBand(shape, parent)
{
}

QPixmap ProcRubberBand::pixmap() const
{
    return m_pixmap;
}

void ProcRubberBand::setPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;
}

void ProcRubberBand::paintEvent(QPaintEvent * event)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_pixmap);
    p.end();

    QRubberBand::paintEvent(event);
}
