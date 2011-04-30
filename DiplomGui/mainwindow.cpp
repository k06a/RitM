#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProcTableWidgetItem.h"
#include "ModuleHolder.h"
#include "IAdapter.h"
#include "IAdapterModule.h"
#include <QVariant>
#include <QCheckBox>
#include <QUndoStack>
#include <QUndoView>
#include <QSettings>
#include <QClipboard>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tableZoom(1.0)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configure Undo/Redo actions

    m_stack = new QUndoStack(this);
    m_action_undo = m_stack->createUndoAction(this,tr("Отменить"));
    m_action_redo = m_stack->createRedoAction(this,tr("Повторить"));
    m_action_undo->setShortcut(QKeySequence::Undo);
    m_action_redo->setShortcut(QKeySequence::Redo);
    m_action_undo->setIcon(QIcon(":/images/undo.png"));
    m_action_redo->setIcon(QIcon(":/images/redo.png"));

    ui->menu_edit->insertAction(ui->action_cut, m_action_undo);
    ui->menu_edit->insertAction(ui->action_cut, m_action_redo);
    ui->menu_edit->insertSeparator(ui->action_cut);
    ui->mainToolBar->insertAction(ui->action_cut, m_action_undo);
    ui->mainToolBar->insertAction(ui->action_cut, m_action_redo);
    ui->mainToolBar->insertSeparator(ui->action_cut);

    ui->tableWidget_field->setStack(m_stack);

    QUndoView * undo_view = new QUndoView(m_stack);
    undo_view->setEmptyLabel(tr("<Исходное состояние>"));
    undo_view->setCleanIcon(ui->action_save->icon());
    ui->dockWidget_undo->setWidget(undo_view);
    ui->dockWidget_undo->hide();

    // Configure table zoom

    ui->tableWidget_field->setBaseZoomWidth(32*5);
    ui->tableWidget_field->setBaseZoomHeight(32*2);
    ui->tableWidget_field->setMinimumZoom(0.5);
    ui->tableWidget_field->setMaximumZoom(5.0);
    ui->tableWidget_field->setZoomStep(0.2);
    ui->tableWidget_field->setCurrentZoom(1.0);

    connect(ui->action_zoomin, SIGNAL(triggered()), ui->tableWidget_field, SLOT(zoomIn()));
    connect(ui->action_zoomout, SIGNAL(triggered()), ui->tableWidget_field, SLOT(zoomOut()));

    // Cut Copy Paste

    connect(ui->action_cut,   SIGNAL(triggered()), ui->tableWidget_field, SLOT(cutSlot()));
    connect(ui->action_copy,  SIGNAL(triggered()), ui->tableWidget_field, SLOT(copySlot()));
    connect(ui->action_paste, SIGNAL(triggered()), ui->tableWidget_field, SLOT(pasteSlot()));

    //

    connect(ui->action_new, SIGNAL(triggered()), this, SLOT(clearTable()));
    connect(ui->action_open, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->action_save, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->action_saveas, SIGNAL(triggered()), this, SLOT(saveAs()));

    connect(ui->action_selectall, SIGNAL(triggered()), ui->tableWidget_field, SLOT(selectAll()));
    connect(ui->action_delete, SIGNAL(triggered()), ui->tableWidget_field, SLOT(deleteSelectedItems()));
    connect(ui->action_about, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_quit, SIGNAL(triggered()), this, SLOT(close()));

    // Add ToolBars and DockWidgets Actions to Windows Menu
    foreach(QToolBar * toolBar, findChildren<QToolBar*>())
        ui->menu_widgets->addAction(toolBar->toggleViewAction());
    ui->menu_widgets->addSeparator();
    foreach(QDockWidget * dockWidget, findChildren<QDockWidget*>())
        ui->menu_widgets->addAction(dockWidget->toggleViewAction());

    ui->listWidget_elements->clear();
    ModuleHolder * holder = ModuleHolder::instance(ui->listWidget_elements);
    //holder->addModule(tr(":/images/processor.svg"),"Processor","Base");
    //holder->addModule(tr(":/images/adapter.svg"),"Adapter","Base");
    //holder->addModule(tr(":/images/connector.svg"),"Connector","Base");

    // Add all pipes
    holder->addModule(Direction::LeftRight,      "Pipe", "Left2Right", tr(":/images/pipes/Left2Right.svg"));
    holder->addModule(Direction::TopBottom,      "Pipe", "Top2Bottom", tr(":/images/pipes/Top2Bottom.svg"));
    holder->addModule(Direction::LeftTopBottom,  "Pipe", "Left2TopBottom", tr(":/images/pipes/Left2TopBottom.svg"));
    holder->addModule(Direction::TopBottomRight, "Pipe", "TopBottom2Right", tr(":/images/pipes/TopBottom2Right.svg"));
    holder->addModule(Direction::LeftTop,        "Pipe", "Left2Top", tr(":/images/pipes/Left2Top.svg"));
    holder->addModule(Direction::LeftBottom,     "Pipe", "Left2Bottom", tr(":/images/pipes/Left2Bottom.svg"));
    holder->addModule(Direction::TopRight,       "Pipe", "Top2Right", tr(":/images/pipes/Top2Right.svg"));
    holder->addModule(Direction::BottomRight,    "Pipe", "Bottom2Right", tr(":/images/pipes/Bottom2Right.svg"));

    ui->listWidget_elements->setSlider(ui->horizontalSlider_elements);
    ui->listWidget_pipes->setSlider(ui->horizontalSlider_elements);

    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardChanged()));
    connect(m_stack, SIGNAL(indexChanged(int)), this, SLOT(stackChanged()));

    stackChanged();
    clipboardChanged();
    on_tableWidget_field_itemSelectionChanged();

    QStringList arg = QApplication::arguments();
    if (arg.size() > 1)
        openFile(arg[1]);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent * event)
{
    QSettings settings("RitM.ini", QSettings::IniFormat);
    if (settings.contains("geometry"))
        restoreGeometry(settings.value("geometry").toByteArray());
    if (settings.contains("windowState"))
        restoreState(settings.value("windowState").toByteArray());
    if (settings.contains("elementsZoom"))
        ui->horizontalSlider_elements->setValue(settings.value("elementsZoom").toInt());
    if (settings.contains("fieldZoom"))
        ui->tableWidget_field->setCurrentZoom(settings.value("fieldZoom").toFloat());
    QMainWindow::showEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!saveOrCancel())
    {
        event->ignore();
        return;
    }
    QSettings settings("RitM.ini", QSettings::IniFormat);
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("elementsZoom", ui->horizontalSlider_elements->value());
    settings.setValue("fieldZoom", ui->tableWidget_field->currentZoom());
    QMainWindow::closeEvent(event);
}

void MainWindow::stackChanged()
{
    bool b = (m_stack->index() > 0);
    ui->action_save->setEnabled(b);
}

void MainWindow::clipboardChanged()
{
    const QMimeData * mime = QApplication::clipboard()->mimeData();
    bool b = (mime->formats().size() > 0) && (mime->formats().first() == "RitM/processors");
    ui->action_paste->setEnabled(b);
}

bool MainWindow::saveOrCancel()
{
    if (!m_action_undo->isEnabled())
        return true;

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Уведомление"));
    msgBox.setText(tr("Документ был изменён"));
    msgBox.setInformativeText(tr("Сохранить изменения в файле?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setButtonText(QMessageBox::Save, tr("Сохранить"));
    msgBox.setButtonText(QMessageBox::Discard, tr("Не сохранять"));
    msgBox.setButtonText(QMessageBox::Cancel, tr("Отмена"));
    int ret = msgBox.exec();

    switch (ret)
    {
        case QMessageBox::Save:
            if (!save())
                return false;
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return false;
    }

    return true;
}

void MainWindow::clearTableWithoutCheck()
{
    ui->tableWidget_field->clear();
    setWindowTitle(tr("RitM in the Middle"));
    m_stack->clear();
}

bool MainWindow::clearTable()
{
    if (!saveOrCancel())
        return false;

    clearTableWithoutCheck();
    return true;
}

bool MainWindow::save()
{
    if (m_filename == "")
        return saveAs();

    QFile file(m_filename);
    file.open(QIODevice::WriteOnly);
    file.write(ui->tableWidget_field->save());
    file.close();

    m_stack->clear();
    return true;
}

bool MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(
        this, tr("Сохранить как ..."), QString(), tr("Структура тракта (*.ritm)"));
    if (filename.isEmpty())
        return false;

    m_filename = filename;
    if (save())
    {
        setWindowTitle(tr("%1 - RitM in the Middle").arg(m_filename));
        return true;
    }

    return false;
}

bool MainWindow::open()
{
    if (!saveOrCancel())
        return false;

    QString filename = QFileDialog::getOpenFileName(
        this, tr("Открыть ..."), QString(), tr("Структура тракта (*.ritm)"));
    if (filename.isEmpty())
        return false;

    clearTableWithoutCheck();
    openFile(filename);
    return true;
}

bool MainWindow::openFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    try
    {
        ui->tableWidget_field->load(file.readAll());
    }
    catch(...)
    {
        QMessageBox::warning(this, tr("Ошибка открытия файла"),
                             tr("Выбран файл неверного формата"));
        return false;
    }
    file.close();

    m_filename = filename;
    setWindowTitle(tr("%1 - RitM in the Middle").arg(m_filename));

    m_stack->clear();
    return true;
}

void MainWindow::on_horizontalSlider_elements_valueChanged(int value)
{
    if (value == ui->horizontalSlider_elements->minimum()
        && ui->listWidget_elements->viewMode() == QListView::IconMode)
    {
        ui->listWidget_elements->setViewMode(QListView::ListMode);
        ui->listWidget_elements->setIconSize(QSize(40,16));
        ui->listWidget_pipes->setViewMode(QListView::ListMode);
        ui->listWidget_pipes->setIconSize(QSize(40,16));
        ui->listWidget_elements->setGridSize(QSize(40,16));
        ui->listWidget_pipes->setGridSize(QSize(40,16));
        return;
    }

    if (value > ui->horizontalSlider_elements->minimum()
        && ui->listWidget_elements->viewMode() == QListView::ListMode)
    {
        ui->listWidget_elements->setViewMode(QListView::IconMode);
        ui->listWidget_pipes->setViewMode(QListView::IconMode);
    }

    ui->listWidget_elements->setIconSize(QSize(value,value));
    ui->listWidget_elements->setGridSize(QSize(value*1.4*2.5,value*1.4));
    ui->listWidget_pipes->setIconSize(QSize(value,value));
    ui->listWidget_pipes->setGridSize(QSize(value*1.4*2.5,value*1.4));
}

void MainWindow::on_tableWidget_field_itemSelectionChanged()
{
    int count = ui->tableWidget_field->nonEmptySelectedItems().size();
    ui->action_cut->setEnabled(count > 0);
    ui->action_copy->setEnabled(count > 0);
    ui->action_delete->setEnabled(count > 0);
}

void MainWindow::on_action_check_triggered()
{
    struct TableCell
    {
        int row;
        int column;
        ProcTableWidgetItem * item;
    };

    QList<TableCell> adapters;
    QList<TableCell> connectors;
    QList<TableCell> processors;
    QList<TableCell> pipes;

    for (int i = 0; i < ui->tableWidget_field->rowCount(); i++)
    for (int j = 0; j < ui->tableWidget_field->columnCount(); j++)
    {
        ProcTableWidgetItem * w =
            qobject_cast<ProcTableWidgetItem*>(
                ui->tableWidget_field->cellWidget(i,j));
        if (w == NULL)
            continue;

        TableCell tc = {i,j,w};

        QString s = w->text();
        if (w->moduleFullName().startsWith("Pipe."))
        {
            pipes.append(tc);
            continue;
        }

        ModuleRecord * rec = w->record();
        if (rec->module.adapterModule != NULL)
            adapters.append(tc);
        if (rec->module.connectorModule != NULL)
            connectors.append(tc);
        if (rec->module.processorModule != NULL)
            processors.append(tc);
    }

    if (adapters.size() == 0)
    {
        QMessageBox::warning(this, tr("Ошибка в построении тракта"),
            tr("В тракте отсутствуют источники трафика. Необходимо "
               "использовать хотя бы один объект адаптера."));
        return;
    }

    foreach(TableCell admod, adapters)
    {
        AdapterPtr ad = admod.item->record()->module.adapterModule->createAdapter();
        //
        ad->ping(ProcessorPtr());
    }
}

void MainWindow::on_action_start_triggered()
{

}

void MainWindow::on_action_stop_triggered()
{

}
