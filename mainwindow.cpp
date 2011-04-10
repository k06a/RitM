#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProcTableWidgetItem.h"
#include "ModuleHolder.h"
#include <QVariant>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tableZoom(1.0)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->action_zoomin->setShortcut(QKeySequence::ZoomIn);
    //ui->action_zoomout->setShortcut(QKeySequence::ZoomOut);
    ui->tableWidget_field->setBaseZoomWidth(64);
    ui->tableWidget_field->setBaseZoomHeight(64);
    ui->tableWidget_field->setMinimumZoom(0.5);
    ui->tableWidget_field->setMaximumZoom(5.0);
    ui->tableWidget_field->setZoomStep(0.2);
    ui->tableWidget_field->setCurrentZoom(1.0);

    connect(ui->action_zoomin, SIGNAL(triggered()), ui->tableWidget_field, SLOT(zoomIn()));
    connect(ui->action_zoomout, SIGNAL(triggered()), ui->tableWidget_field, SLOT(zoomOut()));

    // Add ToolBars and DockWidgets Actions to Windows Menu
    foreach(QToolBar * toolBar, findChildren<QToolBar*>())
        ui->menu_widgets->addAction(toolBar->toggleViewAction());
    ui->menu_widgets->addSeparator();
    foreach(QDockWidget * dockWidget, findChildren<QDockWidget*>())
        ui->menu_widgets->addAction(dockWidget->toggleViewAction());

    ModuleHolder * holder = ModuleHolder::instance();
    holder->addModule(NULL,QPixmap(":/images/processor.svgz"),"Processor","Base");
    holder->addModule(NULL,QPixmap(":/images/adapter.svgz"),"Adapter","Base");
    holder->addModule(NULL,QPixmap(":/images/connector.svgz"),"Connector","Base");

    // Add all pipes
    holder->addModule(NULL,QPixmap(":/images/pipes/Left2Right.svg"),"Left2Right","Pipe");
    holder->addModule(NULL,QPixmap(":/images/pipes/Top2Bottom.svg"),"Top2Bottom","Pipe");
    holder->addModule(NULL,QPixmap(":/images/pipes/Left2TopBottom.svg"),"Left2TopBottom","Pipe");
    holder->addModule(NULL,QPixmap(":/images/pipes/TopBottom2Right.svg"),"TopBottom2Right","Pipe");
    holder->addModule(NULL,QPixmap(":/images/pipes/Left2Top.svg"),"Left2Top","Pipe");
    holder->addModule(NULL,QPixmap(":/images/pipes/Left2Bottom.svg"),"Left2Bottom","Pipe");
    holder->addModule(NULL,QPixmap(":/images/pipes/Top2Right.svg"),"Top2Right","Pipe");
    holder->addModule(NULL,QPixmap(":/images/pipes/Bottom2Right.svg"),"Bottom2Right","Pipe");

    ui->listWidget_elements->setSlider(ui->horizontalSlider);
    ui->listWidget_connections->setSlider(ui->horizontalSlider);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
void MainWindow::updateItemSize(int size)
{
    ui->tableWidget_field->setIconSize(QSize(size-4,size-4));
    ui->tableWidget_field->horizontalHeader()->setDefaultSectionSize(size);
    ui->tableWidget_field->verticalHeader()->setDefaultSectionSize(size);
}

void MainWindow::on_action_zoomin_triggered()
{
    if (tableZoom < 5.00)
    {
        tableZoom *= 1.20;
        updateItemSize(64 * tableZoom);
    }
}

void MainWindow::on_action_zoomout_triggered()
{
    if (tableZoom > 0.50)
    {
        tableZoom *= 0.80;
        updateItemSize(64 * tableZoom);
    }
}
*/

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->listWidget_elements->setIconSize(QSize(value,value));
    ui->listWidget_elements->setGridSize(QSize(value*1.4,value*1.4));
    ui->listWidget_connections->setIconSize(QSize(value,value));
    ui->listWidget_connections->setGridSize(QSize(value*1.4,value*1.4));
}
