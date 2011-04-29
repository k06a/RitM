#ifndef MIMEDATA_H
#define MIMEDATA_H

#include <QMimeData>
#include "ModuleHolder.h"

class ProcTableWidget;
class QRubberBand;

class ProcMimeData : public QMimeData
{
    Q_OBJECT

    Q_PROPERTY(QString m_moduleName
               READ moduleName
               WRITE setModuleName)

    Q_PROPERTY(ProcTableWidget * m_tableWidget
               READ tableWidget
               WRITE setTableWidget)

    Q_PROPERTY(Module m_module
               READ module
               WRITE setModule)

    QString m_moduleName;
    ProcTableWidget * m_tableWidget;
    Module m_module;

public:
    ProcMimeData();

    QString moduleName() const;
    void setModuleName(QString moduleName);

    ProcTableWidget * tableWidget() const;
    void setTableWidget(ProcTableWidget * tableWidget);

    Module module() const;
    void setModule(Module module);

signals:

public slots:

};

#endif // MIMEDATA_H
