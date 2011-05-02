#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>

#include "CommonInclude.h"
#include "IProcessor.h"

class RitmThread;
class QUndoStack;
class ProcTableWidgetItem;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct TableCell;

    QList<TableCell> adapters;

    double tableZoom;
    QUndoStack * m_stack;
    QAction * m_action_undo;
    QAction * m_action_redo;
    QString m_filename;

    RitmThread * thread;
    DiplomBukov::StarterPtr m_starter;
    int m_refreshId;
    QSet<ProcTableWidgetItem*> m_refreshCells;

    struct TableCell
    {
        int row;
        int column;
        ProcTableWidgetItem * item;

        bool operator == (const TableCell & cell)
        {
            return (row == cell.row) && (column == cell.column);
        }
    };

    struct TractStat
    {
        int procs;
        int conns;
        int pipes;

        TractStat(int procs = 0,
                  int conns = 0,
                  int pipes = 0)
            : procs(procs)
            , conns(conns)
            , pipes(pipes)
        {
        }

        TractStat & operator += (const TractStat & stat)
        {
            procs += stat.procs;
            conns += stat.conns;
            pipes += stat.pipes;
            return (*this);
        }
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void showEvent(QShowEvent * event);
    virtual void closeEvent(QCloseEvent * event);
    virtual void timerEvent(QTimerEvent * event);

private slots:
    void stackChanged();
    void clipboardChanged();
    bool saveOrCancel();
    void clearTableWithoutCheck();
    bool clearTable();
    bool save();
    bool saveAs();
    bool open();
    bool openFile(QString filename);
    void on_horizontalSlider_elements_valueChanged(int value);
    void on_tableWidget_field_itemSelectionChanged();

    bool on_action_check_triggered(bool silentOnSuccess);
    void on_action_start_triggered();
    void on_action_stop_triggered();

    TractStat connectRecursive(DiplomBukov::ProcessorPtr nowProc, QList<TableCell> cells, int r, int c, int dx, int dy);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
