#include "ProcTableWidgetItem.h"
#include <QPalette>
#include <QLayout>
#include <QLabel>
#include <QPainter>

ProcTableWidgetItem::ProcTableWidgetItem(QWidget *parent)
    : QWidget(parent)
{
}

ProcTableWidgetItem::ProcTableWidgetItem(QString iconPath, QString centerText, QWidget * parent)
    : QWidget(parent), m_text(centerText), m_pixmap(iconPath)
{
    setAutoFillBackground(true);
    setAttribute(Qt::WA_NoBackground, false);
    setAttribute(Qt::WA_NoSystemBackground, false);

    QHBoxLayout * hlay = new QHBoxLayout(this);
    QLabel * label = new QLabel(centerText);
    label->setAlignment(Qt::AlignCenter);
    hlay->addWidget(label);
}

ProcTableWidgetItem::ProcTableWidgetItem(ProcTableWidgetItem * item)
    : m_text(item->m_text), m_pixmap(item->m_pixmap)
{
    setAutoFillBackground(true);
    setAttribute(Qt::WA_NoBackground, false);
    setAttribute(Qt::WA_NoSystemBackground, false);

    QHBoxLayout * hlay = new QHBoxLayout(this);
    QLabel * label = new QLabel(m_text);
    label->setAlignment(Qt::AlignCenter);
    hlay->addWidget(label);
}

QString ProcTableWidgetItem::text() const
{
    return m_text;
}

void ProcTableWidgetItem::setText(QString text)
{
    m_text = text;
}

QPixmap ProcTableWidgetItem::pixmap() const
{
    return m_pixmap;
}

void ProcTableWidgetItem::setPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;
}

void ProcTableWidgetItem::paintEvent(QPaintEvent * event)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_pixmap);
    p.end();

    QWidget::paintEvent(event);
}
