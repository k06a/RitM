#ifndef RITMTHREAD_H
#define RITMTHREAD_H

#include "CommonInclude.h"
#include <QThread>

using DiplomBukov::StarterPtr;

class RitmThread : public QThread
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

private:
    
};

#endif // RITMTHREAD_H
