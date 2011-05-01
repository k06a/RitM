#include "ProcTableWidget.h"
#include <QApplication>
#include <QMimeData>
#include <QHeaderView>
#include <QListWidgetItem>
#include <QModelIndexList>
#include <QPainter>
#include <QDebug>
#include <QClipboard>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QPushButton>

#include "ProcTableWidgetItem.h"
#include "ProcMimeData.h"
#include "ModuleHolder.h"
#include "ProcCommands.h"
#include "QtOptionWalker.h"
#include "IProcessor.h"
#include "IAdapter.h"
#include "IConnector.h"
#include "IStatsProvider.h"

ProcTableWidget::ProcTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenu(QPoint)));
}

int ProcTableWidget::baseZoomWidth() const
{
    return m_baseZoomWidth;
}

void ProcTableWidget::setBaseZoomWidth(int value)
{
    m_baseZoomWidth = value;
}

int ProcTableWidget::baseZoomHeight() const
{
    return m_baseZoomHeight;
}

void ProcTableWidget::setBaseZoomHeight(int value)
{
    m_baseZoomHeight = value;
}

float ProcTableWidget::minimumZoom() const
{
    return m_minimumZoom;
}

void ProcTableWidget::setMinimumZoom(float value)
{
    m_minimumZoom = value;
}

float ProcTableWidget::maximumZoom() const
{
    return m_maximumZoom;
}

void ProcTableWidget::setMaximumZoom(float value)
{
    m_maximumZoom = value;
}

float ProcTableWidget::zoomStep() const
{
    return m_zoomStep;
}

void ProcTableWidget::setZoomStep(float value)
{
    m_zoomStep = value;
}

float ProcTableWidget::currentZoom() const
{
    return m_currentZoom;
}

void ProcTableWidget::setCurrentZoom(float value)
{
    m_currentZoom = value;
    if (m_currentZoom > m_maximumZoom)
        m_currentZoom = m_maximumZoom;
    if (m_currentZoom < m_minimumZoom)
        m_currentZoom = m_minimumZoom;
    zoomTo(m_currentZoom);
}

QUndoStack * ProcTableWidget::stack() const
{
    return m_stack;
}

void ProcTableWidget::setStack(QUndoStack * stack)
{
    m_stack = stack;
}

// Cut Copy Paste

QString ProcTableWidget::cut()
{
    QString str = copy();
    deleteSelectedItems();
    return str;
}

QString ProcTableWidget::copy() const
{
    QList<QTableWidgetItem*> itemList = nonEmptySelectedItems();
    if (itemList.size() == 0)
        return "";

    QList<ProcItem> procList;
    foreach(QTableWidgetItem * item, itemList)
    {
        QWidget * w = cellWidget(item->row(), item->column());
        ProcTableWidgetItem * wi = (ProcTableWidgetItem*)w;
        ProcItem it(item->row(), item->column(),
                    new ProcTableWidgetItem(wi));
        procList.push_back(it);
    }

    QString str = CopyProcCommand(0, procList, 0, 0, 0).toStringForm();

    return str;
}

void ProcTableWidget::paste(QString str)
{
    int r = currentRow();
    int c = currentColumn();
    m_stack->push(new CopyProcCommand(this, str, 0, r, c));
}

void ProcTableWidget::load(QByteArray arr)
{
    QStringList itemList = QString::fromUtf8(arr).split("|RxC|");
    if (itemList.size() < 2)
        throw "";
    setRowCount(itemList.takeFirst().toInt());
    setColumnCount(itemList.takeFirst().toInt());

    foreach(QString it, itemList)
    {
        ProcItem pi(it);
        setCellWidget(pi.row, pi.column, pi.widget);
        if (item(pi.row, pi.column) == NULL)
            setItem(pi.row, pi.column, new QTableWidgetItem);
    }
}

QByteArray ProcTableWidget::save()
{
    QStringList itemList;
    itemList.append(tr("%1").arg(rowCount()));
    itemList.append(tr("%1").arg(columnCount()));
    for (int i = 0; i < rowCount(); i++)
    for (int j = 0; j < columnCount(); j++)
    {
        ProcTableWidgetItem * w = (ProcTableWidgetItem*)cellWidget(i,j);
        if (w != NULL)
        {
            ProcItem pi(i,j,w);
            itemList.append(pi.toString());
        }
    }

    return itemList.join("|RxC|").toUtf8();
}

QList<QTableWidgetItem*> ProcTableWidget::nonEmptySelectedItems() const
{
    QList<QTableWidgetItem*> list;
    foreach(QModelIndex index, selectedIndexes())
        if (cellWidget(index.row(),index.column()) != 0)
            list.append(itemFromIndex(index));
    return list;
}

// private

QRect ProcTableWidget::itemsBoundingRect(QList<QTableWidgetItem*> list) const
{
    QRect answer;
    foreach(QTableWidgetItem * item, list)
    {
        int top  = rowViewportPosition(item->row());
        int left = columnViewportPosition(item->column());

        QWidget * w = cellWidget(item->row(), item->column());
        QRect rect(QPoint(left, top),
                   QPoint(left + w->size().width(),
                          top + w->size().height()));

        if (answer.isEmpty())
        {
            answer = rect;
            continue;
        }

        answer.setLeft  ( qMin( answer.left()  , rect.left()   ) );
        answer.setTop   ( qMin( answer.top()   , rect.top()    ) );
        answer.setRight ( qMax( answer.right() , rect.right()  ) );
        answer.setBottom( qMax( answer.bottom(), rect.bottom() ) );
    }
    return answer;
}

// virtual protected

void ProcTableWidget::wheelEvent(QWheelEvent * event)
{
    if (!(event->modifiers() & Qt::ControlModifier))
    {
        QTableWidget::wheelEvent(event);
        return;
    }

    int delta = event->delta()/120;
    if (delta > 0)
    {
        for (int i = 0; i < delta; i++)
            zoomIn();
    }
    else
    {
        for (int i = 0; i < -delta; i++)
            zoomOut();
    }

    event->accept();
}

void ProcTableWidget::mousePressEvent(QMouseEvent * event)
{
    m_firstTouch = event->pos();
    m_lastTouch.setY(event->pos().y());
    m_lastTouch.setX(event->pos().x());

    if (!(event->modifiers() & Qt::ControlModifier))
    {
        int pos = selectedIndexes().indexOf(indexAt(event->pos()));
        if (pos != -1)
            m_waitForMove = true;
    }

    QTableWidget::mousePressEvent(event);
}

void ProcTableWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_firstTouch).manhattanLength()
        < QApplication::startDragDistance())
        return;

    QTableWidgetItem * it = itemAt(m_firstTouch);
    if (it == NULL) return;
    if (!itemAt(m_firstTouch)->isSelected())
    {
        clearSelection();
        return;
    }
    if (NULL == cellWidget(rowAt(m_firstTouch.y()),
                           columnAt(m_firstTouch.x())))
    {
        return;
    }


    // Get non-empty draggable items
    m_dragItems = nonEmptySelectedItems();
    if (m_dragItems.size() == 0)
        return;

    QRect bigrect = itemsBoundingRect(m_dragItems);

    // Draw drag icons
    QPixmap pixmap(bigrect.size());
    pixmap.fill(QColor(0,0,0,0));
    QPainter p(&pixmap);
    foreach(QTableWidgetItem * item, m_dragItems)
    {
        QWidget * w = cellWidget(item->row(), item->column());
        ProcTableWidgetItem * twi = qobject_cast<ProcTableWidgetItem*>(w);

        int top  = rowViewportPosition(item->row());
        int left = columnViewportPosition(item->column());
        QPoint tl = QPoint(left, top) - bigrect.topLeft();
        QRect rect = QRect(tl, w->size());

        //QPoint tl = w->rect().topLeft() - bigrect.topLeft();
        //QRect rect = QRect(tl, w->size());

        p.drawPixmap(rect, twi->pixmap());
        p.setPen(QColor(0,0,192));
        p.drawRect(rect);
        p.fillRect(rect, QBrush(QColor(0,0,192,128), Qt::Dense4Pattern));
    }
    p.end();

    ProcMimeData * mimeData = new ProcMimeData;
    QDrag * drag = new QDrag(this);
    drag->setHotSpot(m_firstTouch - bigrect.topLeft());
    drag->setPixmap(pixmap);
    drag->setMimeData(mimeData);

    // drag data

    m_touchItem = itemAt(event->pos());
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

    Q_UNUSED(dropAction);
}

void ProcTableWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (m_waitForMove)
    {
        QTableWidgetItem * it = itemAt(event->pos());
        if (it == NULL)
        {
            it = new QTableWidgetItem;
            setItem(rowAt(event->pos().y()),
                    columnAt(event->pos().x()),it);
        }

        clearSelection();
        it->setSelected(true);
        m_waitForMove = false;
    }
}

void ProcTableWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    processorPropertiesAction();
}

Qt::DropActions ProcTableWidget::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

void ProcTableWidget::dragEnterEvent(QDragEnterEvent * event)
{
    //if (event->source() != this)
    //{
        const ProcMimeData * mimeData =
            qobject_cast<const ProcMimeData*>(event->mimeData());
        if (mimeData == NULL)
            return;
    //}
    event->acceptProposedAction();
}

void ProcTableWidget::dragMoveEvent(QDragMoveEvent * event)
{
    const ProcMimeData * mimeData =
        qobject_cast<const ProcMimeData*>(event->mimeData());
    if (mimeData == NULL)
        return;
    if ((rowAt(event->pos().y()) == rowAt(m_lastTouch.y())) &&
        (columnAt(event->pos().x()) == columnAt(m_lastTouch.x())) &&
        (m_dragItems.size() != 0))
    {
        return;
    }

    m_waitForMove = false;

    if (event->source() != this)
    {
        if (!this->hasFocus())
            this->setFocus();
        clearSelection();

        QTableWidgetItem * it = itemAt(event->pos());
        if (it == NULL)
        {
            it = new QTableWidgetItem();
            setItem(rowAt(event->pos().y()),
                    columnAt(event->pos().x()), it);
        }
        it->setSelected(true);
    }
    else
    {
        clearSelection();

        int dr = rowAt(event->pos().y()) - m_touchItem->row();
        int dc = columnAt(event->pos().x()) - m_touchItem->column();

        for (int i = 0; i < m_dragItems.size(); i++)
        {
            int r = m_dragItems[i]->row() + dr;
            int c = m_dragItems[i]->column() + dc;

            if ((r < 0) || (c < 0))
            {
                clearSelection();
                event->ignore();
                return;
            }

            QTableWidgetItem * it = item(r,c);
            if (it == NULL)
            {
                it = new QTableWidgetItem;
                setItem(r, c, it);
            }

            it->setSelected(true);
        }
    }

    m_lastTouch.setY(rowAt(event->pos().y()));
    m_lastTouch.setX(columnAt(event->pos().x()));
}

void ProcTableWidget::dragLeaveEvent(QDragLeaveEvent * event)
{
    clearSelection();
}

void ProcTableWidget::dropEvent(QDropEvent * event)
{
    const ProcMimeData * mimeData =
        qobject_cast<const ProcMimeData*>(event->mimeData());
    if (mimeData == NULL)
        return;

    if (event->source() != this)
    {
        setCurrentCell(rowAt(event->pos().y()),
                       columnAt(event->pos().x()));

        ModuleHolder * holder = ModuleHolder::instance();
        ModuleRecord * rec = holder->moduleForName(mimeData->moduleName());
        ProcTableWidgetItem * w = new ProcTableWidgetItem();
        w->setPixmap(rec->pixmapPath);
        w->setText(tr("[%1]").arg(rec->name));
        w->setModuleFullName(rec->fullName());
        ProcRecord procRec(rec->module);
        w->setProcRecord(procRec);

        // Разбиение строки для ToolTip-а
        QString str = tr(procRec.info);
        QString tip = "";
        while (str.length() > 40)
        {
            int pos = 40;
            while ((str[pos] != ' ') && (pos < str.length())) pos++;
            while (!QChar(str[pos]).isLetter() && (pos < str.length())) pos++;
            tip += str.left(pos) + "\n";
            str.remove(0, pos);
        }
        tip += str;
        itemAt(event->pos())->setToolTip(tip);

        ProcItem proc(rowAt(event->pos().y()),
                      columnAt(event->pos().x()), w);
        m_stack->push(new PutProcCommand(this, proc));
        setFocus();
    }
    else
    {
        QList<ProcItem> list;
        foreach (QTableWidgetItem * item, m_dragItems)
        {
            int r = item->row();
            int c = item->column();
            ProcTableWidgetItem * w =
                    qobject_cast<ProcTableWidgetItem*>(cellWidget(r,c));
            list.append(ProcItem(r, c, new ProcTableWidgetItem(w)));
        }

        int touchIndex = m_dragItems.indexOf(itemAt(m_firstTouch));
        int putRow = m_lastTouch.y();
        int putColumn = m_lastTouch.x();

        if (event->dropAction() == Qt::CopyAction)
            m_stack->push(new CopyProcCommand(this,list,touchIndex, putRow, putColumn));
        else
        if (event->dropAction() == Qt::MoveAction)
            m_stack->push(new MoveProcCommand(this,list,touchIndex, putRow, putColumn));
    }
}

// slots

void ProcTableWidget::contextMenu(const QPoint & pos)
{
    QTableWidgetItem * it = itemAt(pos);
    if (it == NULL)
        return;

    ProcTableWidgetItem * w =
        qobject_cast<ProcTableWidgetItem*>(cellWidget(it->row(), it->column()));
    if (w == NULL)
        return;

    OptionPtr opts;
    StatsProviderPtr stats;
    if (w->procRecord().adapter)
    {
        opts = w->procRecord().adapter->getOptions();
        stats = w->procRecord().adapter->statsProvider();
    }
    if (w->procRecord().connector)
    {
        opts = w->procRecord().connector->getOptions();
        stats = w->procRecord().connector->statsProvider();
    }
    if (w->procRecord().processor)
    {
        opts = w->procRecord().processor->getOptions();
        stats = w->procRecord().processor->statsProvider();
    }

    QMenu * menu = new QMenu;
    QAction * infoAction = NULL;
    QAction * statsAction = NULL;
    QAction * optionsAction = NULL;

    infoAction = menu->addAction(QIcon(":/images/info.png"),
                                 tr("Информация"), this,
                                 SLOT(processorInfoAction()));
    statsAction = menu->addAction(QIcon(":/images/statistics.png"),
                                  tr("Показатели"), this,
                                  SLOT(processorStatsAction()));
    optionsAction = menu->addAction(QIcon(":/images/options.png"),
                                    tr("Настройки"), this,
                                    SLOT(processorPropertiesAction()));

    if (stats == NULL)
        statsAction->setDisabled(true);
    if (opts == NULL)
        optionsAction->setDisabled(true);

    menu->move(mapToGlobal(pos));
    menu->setDefaultAction(optionsAction);
    menu->exec();
}

void ProcTableWidget::processorPropertiesAction()
{
    QTableWidgetItem * it = this->currentItem();
    if (it == NULL)
        return;

    ProcTableWidgetItem * w =
        qobject_cast<ProcTableWidgetItem*>(cellWidget(it->row(), it->column()));
    if (w == NULL)
        return;
    
    if (w->procRecord().options != NULL)
    {
        QtOptionWalkerPtr walker(new QtOptionWalker());
        w->procRecord().options->visitMe(walker);
        walker->dialog()->setWindowIcon(QIcon(":/images/options.png"));
        walker->dialog()->setWindowTitle(tr("Настройки %1а").arg(w->procRecord().elementName));
        walker->dialog()->exec();
    }
}

void ProcTableWidget::processorStatsAction()
{
    QTableWidgetItem * it = this->currentItem();
    if (it == NULL)
        return;

    ProcTableWidgetItem * w =
        qobject_cast<ProcTableWidgetItem*>(cellWidget(it->row(), it->column()));
    if (w == NULL)
        return;

    StatsProviderPtr stat = w->procRecord().statsProvider;
    
    QDialog * dialog = new QDialog(this);

    // Top Grid Layout

    QGridLayout * topGridLayout = new QGridLayout;
    for (int i = 0; i < stat->getStatistic_size(); i++)
    {
        QLabel * label1 = new QLabel(tr(stat->getStatistic_name(i))+":");
        QLineEdit * le2 = new QLineEdit(tr("%%1").arg(i+1));
        le2->setReadOnly(true);
        //le2->setDisabled(true);
        //le2->setStyleSheet( QString( "background-color: lightgray"));
        topGridLayout->addWidget(label1, i+1, 0);
        topGridLayout->addWidget(le2, i+1, 1);
    }

    // Bottom Grid Layout

    QGridLayout * bottomGridLayout = new QGridLayout;
    
    bottomGridLayout->addWidget(new QLabel(tr("Левый верхний угол:")),  0, 0);
    bottomGridLayout->addWidget(new QLabel(tr("Правый верхний угол:")), 1, 0);
    bottomGridLayout->addWidget(new QLabel(tr("Левый нижний угол:")),   2, 0);
    bottomGridLayout->addWidget(new QLabel(tr("правый нижний угол:")),  3, 0);
    
    QLineEdit * topLeft  = new QLineEdit(w->statText(Direction::LeftTop));
    QLineEdit * topRight = new QLineEdit(w->statText(Direction::TopRight));
    QLineEdit * botLeft  = new QLineEdit(w->statText(Direction::LeftBottom));
    QLineEdit * botRight = new QLineEdit(w->statText(Direction::BottomRight));

    bottomGridLayout->addWidget(topLeft,  0, 1);
    bottomGridLayout->addWidget(topRight, 1, 1);
    bottomGridLayout->addWidget(botLeft,  2, 1);
    bottomGridLayout->addWidget(botRight, 3, 1);

    // Left Layout

    QGroupBox * topGroupBox = new QGroupBox(tr("Показатели модуля"));
    topGroupBox->setLayout(topGridLayout);
    QGroupBox * bottomGroupBox = new QGroupBox(tr("Отображение показателей"));
    bottomGroupBox->setLayout(bottomGridLayout);

    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addWidget(topGroupBox);
    leftLayout->addWidget(bottomGroupBox);
    
    // QDialogButtonBox

    QDialogButtonBox * buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ок"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));
    connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    // Main Layout

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(buttonBox);

    // Dialog

    dialog->setWindowIcon(QIcon(":/images/statistics.png"));
    dialog->setWindowTitle(tr("Показатели статистики"));
    dialog->setLayout(mainLayout);
    int ret = dialog->exec();

    if (ret == QDialog::Rejected)
        return;

    w->setStatText(Direction::LeftTop,     topLeft->text());
    w->setStatText(Direction::TopRight,    topRight->text());
    w->setStatText(Direction::LeftBottom,  botLeft->text());
    w->setStatText(Direction::BottomRight, botRight->text());
}

void ProcTableWidget::processorInfoAction()
{
    QTableWidgetItem * it = this->currentItem();
    if (it == NULL)
        return;

    ProcTableWidgetItem * w =
        qobject_cast<ProcTableWidgetItem*>(cellWidget(it->row(), it->column()));
    if (w == NULL)
        return;

    QDialog * dialog = new QDialog(this);
    dialog->setWindowIcon(QIcon(":/images/info.png"));
    dialog->setWindowTitle(tr("Описание %1а").arg(w->procRecord().elementName));
    
    QLabel * label = new QLabel(tr(w->procRecord().info));
    label->setWordWrap(true);
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ок"));
    connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));

    dialog->setLayout(new QVBoxLayout);
    dialog->layout()->addWidget(label);
    dialog->layout()->addWidget(buttonBox);
    dialog->exec();
}

void ProcTableWidget::cutSlot()
{
    QString str = cut();
    if (str.isEmpty())
        return;

    QMimeData * mime = new QMimeData;
    mime->setData("RitM/processors", str.toUtf8());

    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setMimeData(mime);
}

void ProcTableWidget::copySlot()
{
    QString str = copy();
    if (str.isEmpty())
        return;

    QMimeData * mime = new QMimeData;
    mime->setData("RitM/processors", str.toUtf8());

    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setMimeData(mime);
}

void ProcTableWidget::pasteSlot()
{
    QClipboard * clipboard = QApplication::clipboard();
    const QMimeData * mime = clipboard->mimeData();
    if (mime->formats().first() != "RitM/processors")
        return;

    QString str = QString::fromUtf8(mime->data("RitM/processors"));
    paste(str);
}

void ProcTableWidget::zoomIn()
{
    setCurrentZoom(m_currentZoom + m_zoomStep);
}

void ProcTableWidget::zoomOut()
{
    setCurrentZoom(m_currentZoom - m_zoomStep);
}

void ProcTableWidget::zoomTo(float value)
{
    //int centerX = size().width() / 2;
    //int centerY = size().height() / 2;
    //QTableWidgetItem * centerItem = itemAt(centerX, centerY);

    int w = m_baseZoomWidth * value;
    int h = m_baseZoomHeight * value;
    setIconSize(QSize(w-4,h-4));
    horizontalHeader()->setDefaultSectionSize(w);
    verticalHeader()->setDefaultSectionSize(h);

    //scrollToItem(centerItem, QAbstractItemView::PositionAtCenter);
}

void ProcTableWidget::deleteSelectedItems()
{
    QList<ProcItem> list;
    foreach(QModelIndex index, selectedIndexes())
    {
        int r = index.row();
        int c = index.column();
        ProcTableWidgetItem * w =
                qobject_cast<ProcTableWidgetItem *>(cellWidget(r,c));
        if (w != 0)
            list.append(ProcItem(r,c,new ProcTableWidgetItem(w)));
    }

    if (list.size() != 0)
        m_stack->push(new RemoveProcCommand(this,list));
}
