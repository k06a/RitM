#include <QDebug>
#include "ProcListWidget.h"
#include "ProcMimeData.h"
#include <QPaintEngine>
#include <QApplication>

ProcListWidget::ProcListWidget(QWidget *parent)
    : QListWidget(parent), m_dragFromThis(false)
    , m_rubberBand(QRubberBand::Rectangle, window())
{
}

void ProcListWidget::addProcModule(const ModuleRecord * module)
{
    QListWidgetItem * item =
        new QListWidgetItem(
                QIcon(module->pixmap),
                QString(module->fullName().c_str()));
    item->setToolTip(module->fullName().c_str());
    addItem(item);
}

QSlider * ProcListWidget::slider() const
{
    return m_slider;
}

void ProcListWidget::setSlider(QSlider * slider)
{
    m_slider = slider;
}

void ProcListWidget::wheelEvent(QWheelEvent * event)
{
    if (!(event->modifiers() & Qt::ControlModifier))
    {
        QListWidget::wheelEvent(event);
        return;
    }

    if (m_slider != NULL)
        m_slider->setValue(m_slider->value() + event->delta()/60);
    event->accept();
}

void ProcListWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragStartPosition = event->pos();
        QListWidgetItem * it = itemAt(event->pos());
        if (it == NULL)
        {
            m_dragFromThis = false;
        }
        else
        {
            it->setSelected(true);
            /*
            m_rubberBand.setParent(window());
            m_itemShift = event->pos()
                        - visualItemRect(it).topLeft()
                        - mapTo(window(),pos());
            */
            if (it->isSelected())
                m_dragFromThis = true;
        }
    }
}

void ProcListWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (!(event->buttons() & Qt::LeftButton))
    {
        QListWidget::mouseMoveEvent(event);
        return;
    }
    if ((event->pos() - m_dragStartPosition).manhattanLength()
        < QApplication::startDragDistance())
        return;

    /*
    m_rubberBand.move(event->pos() - m_itemShift);
    if (!m_dragFromThis)
    {
        QListWidgetItem * it = itemAt(event->pos());
        if (it == NULL)
            return;
       /m_rubberBand.setPixmap(it->icon().pixmap(iconSize()));
        m_rubberBand.resize(iconSize());
        m_rubberBand.show();
        m_dragFromThis = true;
    }
    */

    if (!m_dragFromThis)
        return;

    QListWidgetItem * item = selectedItems()[0];
    viewportEntered();

    QPixmap pixmap = item->icon().pixmap(iconSize());
    QPainter p(&pixmap);
    p.setPen(QColor(0,0,192));
    p.drawRect(0,0,pixmap.rect().width()-1,pixmap.rect().height()-1);
    p.fillRect(pixmap.rect(),
               QBrush(QColor(0,0,192,128), Qt::Dense4Pattern));
    p.end();

    ProcMimeData * mimeData = new ProcMimeData;
    mimeData->setModuleName(item->text());
    QDrag * drag = new QDrag(this);
    drag->setHotSpot(m_dragStartPosition - visualItemRect(item).topLeft() - QPoint(8,0));
    drag->setPixmap(pixmap);
    drag->setMimeData(mimeData);

    // drag data

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void ProcListWidget::mouseReleaseEvent(QMouseEvent * event)
{
    m_dragFromThis = false;
    //m_rubberBand.hide();
}
