#ifndef RITMTHREAD_H
#define RITMTHREAD_H

#include "CommonInclude.h"
#include "Log.h"
#include <QThread>

using DiplomBukov::StarterPtr;

class RitmThread
    : public QThread
    , public DiplomBukov::ILogPrinter
{
    Q_OBJECT

    Q_PROPERTY(StarterPtr m_starter
               READ starter
               WRITE setStarter)

    StarterPtr m_starter;

public:
    RitmThread(QObject * parent = NULL);

    StarterPtr starter() const;
    void setStarter(StarterPtr starter);

    virtual void run();
    void stop();

    virtual void printString(const char * str);

signals:
    void printStringSignal(QString str);
};

#endif // RITMTHREAD_H
