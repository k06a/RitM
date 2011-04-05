#include "QIconTableWidget.h"
#include <QMimeData>

QIconTableWidget::QIconTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
}

bool QIconTableWidget::dropMimeData(
    int row, int column,
    const QMimeData * data,
    Qt::DropAction action)
{
    return QTableWidget::dropMimeData(row,column,data,action);

    //if (data->hasImage())
    {
        //QImage image = qvariant_cast<QImage>(data->imageData());
        QIcon icon(":/images/intel.png");
        QTableWidgetItem * item = new QTableWidgetItem();
        item->setBackground(icon.pixmap(64));
        setItem(row, column, item);
        return true;
    }

    return false;
}
