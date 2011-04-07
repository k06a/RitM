#include "TableItem.h"
#include <QPalette>
#include <QLayout>
#include <QLabel>
#include <QPainter>

TableItem::TableItem(QWidget *parent)
    : QWidget(parent)
{
}

TableItem::TableItem(QString iconPath, QString centerText, QWidget * parent)
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

TableItem::TableItem(TableItem * item)
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

void TableItem::paintEvent(QPaintEvent * event)
{
    {
        QPainter p(this);
        p.drawPixmap(rect(), m_pixmap);
    }
    QWidget::paintEvent(event);
}
