#include "ProcTableWidgetItem.h"
#include <QPalette>
#include <QLayout>
#include <QLabel>
#include <QPainter>
#include <QPicture>
#include "ModuleHolder.h"
#include "IStatsProvider.h"
#include "IOption.h"

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
    , m_statPositions(item->m_statPositions)
    , m_moduleFullName(item->m_moduleFullName)
{
}

ProcTableWidgetItem::ProcTableWidgetItem(QString stringForm, int row, int column)
    : QWidget()
{
    QStringList strs = stringForm.split("{@}");

    m_moduleFullName = strs[0];
    ModuleHolder * holder = ModuleHolder::instance();
    ModuleRecord * rec = holder->moduleForName(m_moduleFullName);
    m_pixmapPath = rec->pixmapPath;
    m_pixmap = QPixmap(m_pixmapPath);
    m_procRecord.init(*rec, row, column);
    m_text = "[" + rec->name + "]";

    if (strs.size() == 1)
        return;
    QString optStr = strs[1];
    m_procRecord.options->loadFromString(optStr.toStdString());
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

void ProcTableWidgetItem::setStatText(int direction, QString str)
{
    m_statPositions[direction] = str;
}

QString ProcTableWidgetItem::statText(int direction)
{
    return m_statPositions[direction];
}

void ProcTableWidgetItem::updateStats()
{
    if (m_procRecord.statsProvider == NULL)
        return;

    std::vector<int> stats;
    for (int i = 0; i < m_procRecord.statsProvider->getStatistic_size(); i++)
        stats.push_back(m_procRecord.statsProvider->getStatistic_value(i));

    foreach(int key, m_statPositions.keys())
    {
        QString str = m_statPositions[key];
        for (int i = 0; i < m_procRecord.statsProvider->getStatistic_size(); i++)
            str.replace(tr("%%1").arg(i+1), tr("%1").arg(stats[i]));
        m_statValues[key] = str;
    }
}

QString ProcTableWidgetItem::toStringForm()
{
    QString str = m_moduleFullName;
    if (m_procRecord.options != NULL)
        str += "{@}" + QString::fromStdString(m_procRecord.options->saveToString());
    return str;
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

    QFont font = p.font();
    int size = (rect().width())/18;
    font.setPixelSize(size);
    p.setFont(font);
    
    foreach(int key, m_statValues.keys())
    {
        int align = 0;
        if (key & Direction::Left)   align |= Qt::AlignLeft;
        if (key & Direction::Right)  align |= Qt::AlignRight;
        if (key & Direction::Top)    align |= Qt::AlignTop;
        if (key & Direction::Bottom) align |= Qt::AlignBottom;

        QString str = m_statValues[key];
        p.drawText(rect(), align, str);
    }
    
    if (m_procRecord.adapter != NULL
        || m_procRecord.connector != NULL
        || m_procRecord.processor != NULL)
    {
        if (rect().width() > 80)
            p.drawText(rect(), Qt::AlignCenter, m_text);
    }
    
    p.end();

    QWidget::paintEvent(event);
}
