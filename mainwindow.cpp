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
    /*
    if ((row >= 0) && (column >= 0))
    {
        QList<QTableWidgetItem*> indexes = ui->tableWidget_field->selectedItems();

        if (indexes.size() == 0)
        {
            ui->tableWidget_field->setCurrentCell(
                row, column, QItemSelectionModel::ClearAndSelect);
            return;
        }

        int dragRow = ui->tableWidget_field->currentIndex().row();
        int dragColumn = ui->tableWidget_field->currentIndex().column();
        foreach (QTableWidgetItem * index, indexes)
        {
            int r = index->row() + row - dragRow;
            int c = index->column() + column - dragColumn;

            ui->tableWidget_field->setCurrentCell(
                index->row(), index->column(),
                QItemSelectionModel::Deselect);
            ui->tableWidget_field->setCurrentCell(
                r,c,QItemSelectionModel::Select);
        }
    }
    */
}
