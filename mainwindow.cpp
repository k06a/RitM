#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProcTableWidgetItem.h"
#include "ModuleHolder.h"
#include <QVariant>
#include <QCheckBox>
#include <QUndoStack>
#include <QUndoView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tableZoom(1.0)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configure Undo/Redo actions

    m_stack = new QUndoStack(this);
    m_action_undo = m_stack->createUndoAction(this);
    m_action_redo = m_stack->createRedoAction(this);
    m_action_undo->setText(tr("Отменить"));
    m_action_redo->setText(tr("Повторить"));
    m_action_undo->setShortcut(QKeySequence::Undo);
    m_action_redo->setShortcut(QKeySequence::Redo);
    m_action_undo->setIcon(QIcon(":/images/undo.png"));
    m_action_redo->setIcon(QIcon(":/images/redo.png"));

    ui->menu_edit->insertAction(ui->action_cut, m_action_undo);
    ui->menu_edit->insertAction(ui->action_cut, m_action_redo);
    ui->mainToolBar->insertAction(ui->action_cut, m_action_undo);
    ui->mainToolBar->insertAction(ui->action_cut, m_action_redo);

    ui->tableWidget_field->setStack(m_stack);

    QUndoView * undo_view = new QUndoView(m_stack);
    undo_view->setEmptyLabel(tr("<Исходное состояние>"));
    ui->dockWidget_undo->setWidget(undo_view);
    ui->dockWidget_undo->hide();

    // Configure table zoom

    ui->tableWidget_field->setBaseZoomWidth(64);
    ui->tableWidget_field->setBaseZoomHeight(64);
    ui->tableWidget_field->setMinimumZoom(0.5);
    ui->tableWidget_field->setMaximumZoom(5.0);
    ui->tableWidget_field->setZoomStep(0.2);
    ui->tableWidget_field->setCurrentZoom(1.0);

    connect(ui->action_zoomin, SIGNAL(triggered()), ui->tableWidget_field, SLOT(zoomIn()));
    connect(ui->action_zoomout, SIGNAL(triggered()), ui->tableWidget_field, SLOT(zoomOut()));

    //

    connect(ui->action_delete, SIGNAL(triggered()), ui->tableWidget_field, SLOT(deleteSelectedItems()));
    connect(ui->action_about, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Add ToolBars and DockWidgets Actions to Windows Menu
    foreach(QToolBar * toolBar, findChildren<QToolBar*>())
        ui->menu_widgets->addAction(toolBar->toggleViewAction());
    ui->menu_widgets->addSeparator();
    foreach(QDockWidget * dockWidget, findChildren<QDockWidget*>())
        ui->menu_widgets->addAction(dockWidget->toggleViewAction());

    ModuleHolder * holder = ModuleHolder::instance();
    holder->addModule(NULL,tr(":/images/processor.svg"),"Processor","Base");
    holder->addModule(NULL,tr(":/images/adapter.svg"),"Adapter","Base");
    holder->addModule(NULL,tr(":/images/connector.svg"),"Connector","Base");

    // Add all pipes
    holder->addModule(NULL,tr(":/images/pipes/Left2Right.svg"),"Left2Right","Pipe");
    holder->addModule(NULL,tr(":/images/pipes/Top2Bottom.svg"),"Top2Bottom","Pipe");
    holder->addModule(NULL,tr(":/images/pipes/Left2TopBottom.svg"),"Left2TopBottom","Pipe");
    holder->addModule(NULL,tr(":/images/pipes/TopBottom2Right.svg"),"TopBottom2Right","Pipe");
    holder->addModule(NULL,tr(":/images/pipes/Left2Top.svg"),"Left2Top","Pipe");
    holder->addModule(NULL,tr(":/images/pipes/Left2Bottom.svg"),"Left2Bottom","Pipe");
    holder->addModule(NULL,tr(":/images/pipes/Top2Right.svg"),"Top2Right","Pipe");
    holder->addModule(NULL,tr(":/images/pipes/Bottom2Right.svg"),"Bottom2Right","Pipe");

    ui->listWidget_elements->setSlider(ui->horizontalSlider_elements);
    ui->listWidget_pipes->setSlider(ui->horizontalSlider_elements);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_elements_valueChanged(int value)
{
    ui->listWidget_elements->setIconSize(QSize(value,value));
    ui->listWidget_elements->setGridSize(QSize(value*1.4,value*1.4));
    ui->listWidget_pipes->setIconSize(QSize(value,value));
    ui->listWidget_pipes->setGridSize(QSize(value*1.4,value*1.4));
    ui->horizontalSlider_pipes->setValue(value);
}
