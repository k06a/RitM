#ifndef PROCRUBBERBAND_H
#define PROCRUBBERBAND_H

#include <QRubberBand>

class ProcRubberBand : public QRubberBand
{
    Q_OBJECT

    Q_PROPERTY(QPixmap m_pixmap
               READ pixmap
               WRITE setPixmap)

    QPixmap m_pixmap;

public:
    explicit ProcRubberBand(QRubberBand::Shape shape, QWidget *parent = 0);

    QPixmap pixmap() const;
    void setPixmap(QPixmap pixmap);

protected:
    void paintEvent(QPaintEvent * event);

signals:

public slots:

};

#endif // PROCRUBBERBAND_H
