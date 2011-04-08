#ifndef MIMEDATA_H
#define MIMEDATA_H

#include <QMimeData>

class ProcTableWidget;
class QRubberBand;
class IModule;

class ProcMimeData : public QMimeData
{
    Q_OBJECT

    Q_PROPERTY(QString m_moduleName
               READ moduleName
               WRITE setModuleName)

    Q_PROPERTY(ProcTableWidget * m_tableWidget
               READ tableWidget
               WRITE setTableWidget)

    Q_PROPERTY(IModule * m_module
               READ module
               WRITE setModule)

    QString m_moduleName;
    ProcTableWidget * m_tableWidget;
    IModule * m_module;

public:
    ProcMimeData();

    QString moduleName() const;
    void setModuleName(QString moduleName);

    ProcTableWidget * tableWidget() const;
    void setTableWidget(ProcTableWidget * tableWidget);

    IModule * module() const;
    void setModule(IModule * module);

signals:

public slots:

};

#endif // MIMEDATA_H
