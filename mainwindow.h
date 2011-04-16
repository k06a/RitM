#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QUndoStack;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    double tableZoom;
    QUndoStack * m_stack;
    QAction * m_action_undo;
    QAction * m_action_redo;
    QString m_filename;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void showEvent(QShowEvent * event);
    virtual void closeEvent(QCloseEvent * event);

private slots:
    void clipboardChanged();
    bool checkForSave();
    void clearTableWithoutCheck();
    bool clearTable();
    bool save();
    bool saveAs();
    bool open();
    bool openFile(QString filename);
    void on_horizontalSlider_elements_valueChanged(int value);
    void on_tableWidget_field_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
