#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVariant>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , valueDragged(false)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::on_tableWidget_field_cellChanged(int row, int column)
{
    //if ((row >= 0) && (column >= 0))
    //    ui->tableWidget_field->setCurrentCell(
    //        row, column, QItemSelectionModel::ClearAndSelect);
}
