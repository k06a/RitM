#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QWidget>
#include <QPixmap>

class ProcTableWidgetItem : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString m_text
               READ text
               WRITE setText)

    Q_PROPERTY(QPixmap m_pixmap
               READ pixmap
               WRITE setPixmap)

    QString m_text;
    QPixmap m_pixmap;

    QString m_pixmapPath;

public:
    explicit ProcTableWidgetItem();
    explicit ProcTableWidgetItem(ProcTableWidgetItem * item);
    ProcTableWidgetItem(QString stringForm);
    ProcTableWidgetItem(QString iconPath, QString centerText, QWidget * parent = 0);

    QString text() const;
    void setText(QString text);

    QPixmap pixmap() const;
    void setPixmap(QPixmap pixmap);

    QString toStringForm();
    bool isEqualProc(ProcTableWidgetItem * w);

protected:
    void paintEvent(QPaintEvent * event);

signals:

public slots:

};

#endif // TABLEITEM_H
