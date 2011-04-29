#include "ProcTableWidgetItem.h"
#include <QPalette>
#include <QLayout>
#include <QLabel>
#include <QPainter>
#include <QPicture>
#include "ModuleHolder.h"

ProcTableWidgetItem::ProcTableWidgetItem()
    : QWidget()
{

}

ProcTableWidgetItem::ProcTableWidgetItem(ProcTableWidgetItem * item)
    : QWidget()
    , m_text(item->m_text)
    , m_pixmap(item->m_pixmap)
    , m_pixmapPath(item->m_pixmapPath)
    , m_procRecord(item->m_procRecord)
{
}

ProcTableWidgetItem::ProcTableWidgetItem(QString stringForm)
    : QWidget()
{
    m_moduleFullName = stringForm;
    ModuleHolder * holder = ModuleHolder::instance();
    ModuleRecord * rec = holder->moduleForName(m_moduleFullName);
    m_pixmapPath = rec->pixmapPath;
    m_pixmap = QPixmap(m_pixmapPath);
    m_text = rec->name;
}

ProcTableWidgetItem::ProcTableWidgetItem(QString iconPath, QString centerText, QWidget * parent)
    : QWidget(parent)
    , m_text(centerText)
    , m_pixmap(iconPath)
    , m_moduleFullName(centerText)
    , m_pixmapPath(iconPath)
{
    //setAutoFillBackground(true);
    //setAttribute(Qt::WA_NoBackground, false);
    //setAttribute(Qt::WA_NoSystemBackground, false);

    //QHBoxLayout * hlay = new QHBoxLayout(this);
    //QLabel * label = new QLabel(centerText);
    //label->setAlignment(Qt::AlignCenter);
    //hlay->addWidget(label);
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

QString ProcTableWidgetItem::moduleFullName() const
{
    return m_moduleFullName;
}

void ProcTableWidgetItem::setModuleFullName(QString moduleFullName)
{
    m_moduleFullName = moduleFullName;
}

ProcRecord ProcTableWidgetItem::procRecord() const
{
    return m_procRecord;
}

void ProcTableWidgetItem::setProcRecord(ProcRecord record)
{
    m_procRecord = record;
}

QString ProcTableWidgetItem::toStringForm()
{
    return m_text;
}

bool ProcTableWidgetItem::isEqualProc(ProcTableWidgetItem * w)
{
    return (m_moduleFullName == w->m_moduleFullName);
}

ModuleRecord * ProcTableWidgetItem::record()
{
    ModuleHolder * holder = ModuleHolder::instance();
    return holder->moduleForName(m_text);
}

void ProcTableWidgetItem::paintEvent(QPaintEvent * event)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_pixmap);
    p.end();

    QWidget::paintEvent(event);
}
