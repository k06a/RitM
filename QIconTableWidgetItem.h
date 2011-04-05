#ifndef QICONTABLEWIDGETITEM_H
#define QICONTABLEWIDGETITEM_H

#include <QTableWidgetItem>
#include <QIcon>

class QIconTableWidgetItem : public QObject, public QTableWidgetItem
{
    Q_OBJECT

public:
    explicit QIconTableWidgetItem(const QIcon & icon);

signals:

public slots:

};

#endif // QICONTABLEWIDGETITEM_H
