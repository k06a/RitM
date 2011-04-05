#include "QIconTableWidgetItem.h"

QIconTableWidgetItem::QIconTableWidgetItem(const QIcon & icon)
    : QTableWidgetItem(QTableWidgetItem::UserType + 1)
{
}
