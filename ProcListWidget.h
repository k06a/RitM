#ifndef PROCLISTWIDGET_H
#define PROCLISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>
#include <QRubberBand>
#include "ProcRubberBand.h"
#include "ModuleHolder.h"

class ProcListWidget : public QListWidget
{
    Q_OBJECT

    Q_PROPERTY(QSlider * m_slider
               READ slider
               WRITE setSlider)

    QSlider * m_slider;

    QPoint m_dragStartPosition;
    bool m_dragFromThis;
    QPoint m_itemShift;
    ProcRubberBand m_rubberBand;

public:
    explicit ProcListWidget(QWidget *parent = 0);

    void addProcModule(const ModuleRecord * module);

    QSlider * slider() const;
    void setSlider(QSlider * slider);

protected:
    void wheelEvent(QWheelEvent * event);

    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

signals:

public slots:

};

#endif // PROCLISTWIDGET_H
