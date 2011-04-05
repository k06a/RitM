#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVariant>

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
