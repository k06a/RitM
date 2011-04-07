#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QWidget>
#include <QPixmap>

class TableItem : public QWidget
{
    Q_OBJECT

    QString m_text;
    QPixmap m_pixmap;

public:
    explicit TableItem(QWidget * parent = 0);
    explicit TableItem(QString iconPath, QString centerText = "", QWidget * parent = 0);
    explicit TableItem(TableItem * item);

protected:
    void paintEvent(QPaintEvent * event);

signals:

public slots:

};

#endif // TABLEITEM_H
