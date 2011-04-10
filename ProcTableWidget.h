#ifndef QICONTABLEWIDGET_H
#define QICONTABLEWIDGET_H

#include <QTableWidget>
#include <QDragMoveEvent>

class ProcTableWidget : public QTableWidget
{
    Q_OBJECT

    Q_PROPERTY(int baseZoomWidth
               READ baseZoomWidth
               WRITE setBaseZoomWidth);

    Q_PROPERTY(int baseZoomHeight
               READ baseZoomHeight
               WRITE setBaseZoomHeight);

    Q_PROPERTY(float minimumZoom
               READ minimumZoom
               WRITE setMinimumZoom);

    Q_PROPERTY(float maximumZoom
               READ maximumZoom
               WRITE setMaximumZoom);

    Q_PROPERTY(float zoomStep
               READ zoomStep
               WRITE setZoomStep);

    Q_PROPERTY(float currentZoom
               READ currentZoom
               WRITE setCurrentZoom);

    int m_baseZoomWidth;
    int m_baseZoomHeight;
    float m_minimumZoom;
    float m_maximumZoom;
    float m_zoomStep;
    float m_currentZoom;

    bool m_waitForMove;
    QTableWidgetItem * m_touchItem;
    QPoint m_lastTouchCoord;
    QList<QTableWidgetItem*> m_selectedItems;
    QPoint m_dragStartPosition;

public:
    explicit ProcTableWidget(QWidget *parent = 0);

    int baseZoomWidth() const;
    void setBaseZoomWidth(int value);

    int baseZoomHeight() const;
    void setBaseZoomHeight(int value);

    float minimumZoom() const;
    void setMinimumZoom(float value);

    float maximumZoom() const;
    void setMaximumZoom(float value);

    float zoomStep() const;
    void setZoomStep(float value);

    float currentZoom() const;
    void setCurrentZoom(float value);

protected:
    void wheelEvent(QWheelEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    Qt::DropActions supportedDropActions() const;
    void dragEnterEvent(QDragEnterEvent * event);
    void dragMoveEvent(QDragMoveEvent * event);
    void dragLeaveEvent(QDragLeaveEvent * event);
    void dropEvent(QDropEvent * event);

signals:

public slots:
    void zoomIn();
    void zoomOut();
    void zoomTo(float value);
    void deleteSelectedItems();
};

#endif // QICONTABLEWIDGET_H
