#ifndef MIMEDATA_H
#define MIMEDATA_H

#include <QMimeData>

class QIconTableWidget;

class MimeData : public QMimeData
{
    Q_OBJECT

    Q_PROPERTY(QIconTableWidget * m_tableWidget
               READ tableWidget
               WRITE setTableWidget)

    QIconTableWidget * m_tableWidget;

public:
    MimeData();

    QIconTableWidget * tableWidget() const;
    void setTableWidget(QIconTableWidget * tableWidget);

signals:

public slots:

};

#endif // MIMEDATA_H
