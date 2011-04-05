#ifndef QICONTABLEWIDGET_H
#define QICONTABLEWIDGET_H

#include <QTableWidget>
#include <QDragMoveEvent>

class QIconTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit QIconTableWidget(QWidget *parent = 0);

protected:
    bool dropMimeData(
        int row, int column,
        const QMimeData * data,
        Qt::DropAction action);

signals:

public slots:

};

#endif // QICONTABLEWIDGET_H
