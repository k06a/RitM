#include "MimeData.h"

MimeData::MimeData()
    : QMimeData()
{
}

QIconTableWidget * MimeData::tableWidget() const
{
    return m_tableWidget;
}

void MimeData::setTableWidget(QIconTableWidget * tableWidget)
{
    m_tableWidget = tableWidget;
}
