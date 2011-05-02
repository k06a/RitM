#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QWidget>
#include <QPixmap>
#include <QMap>
#include "ModuleHolder.h"

struct ModuleRecord;

class ProcTableWidgetItem : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString m_text
               READ text
               WRITE setText)

    Q_PROPERTY(QPixmap m_pixmap
               READ pixmap
               WRITE setPixmap)

    Q_PROPERTY(QString m_moduleFullName
               READ moduleFullName
               WRITE setModuleFullName)

    /*
    Q_PROPERTY(ProcRecord m_procRecord
               READ record
               WRITE setRecord)
    */

    QString m_text;
    QPixmap m_pixmap;
    QString m_moduleFullName;
    ProcRecord m_procRecord;

    QString m_pixmapPath;
    QMap<int,QString> m_statPositions;
    QMap<int,QString> m_statValues;

public:
    explicit ProcTableWidgetItem();
    explicit ProcTableWidgetItem(ProcTableWidgetItem * item);
    ProcTableWidgetItem(QString stringForm, int row, int column);
    ProcTableWidgetItem(QString iconPath, QString centerText, QWidget * parent = 0);

    QString text() const;
    void setText(QString text);

    QPixmap pixmap() const;
    void setPixmap(QPixmap pixmap);

    QString moduleFullName() const;
    void setModuleFullName(QString moduleFullName);

    ProcRecord procRecord() const;
    void setProcRecord(ProcRecord procRecord);

    void setStatText(int direction, QString str);
    QString statText(int direction);
    void updateStats();

    QString toStringForm();
    bool isEqualProc(ProcTableWidgetItem * w);
    ModuleRecord * record();

protected:
    void paintEvent(QPaintEvent * event);

signals:

public slots:

};

#endif // TABLEITEM_H
