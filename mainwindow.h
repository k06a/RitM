#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    double tableZoom;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //void updateItemSize(int size);

private slots:
    //void on_action_zoomin_triggered();
    //void on_action_zoomout_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
