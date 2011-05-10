#include "RitmThread.h"
#include "IStarter.h"
#include <iostream>

RitmThread::RitmThread(QObject * parent)
    : QThread(parent)
{
}

StarterPtr RitmThread::starter() const
{
    return m_starter;
}

void RitmThread::setStarter(StarterPtr starter)
{
    m_starter = starter;
}

void RitmThread::run()
{
    try
    {
        m_starter->start();
    }
    catch(std::exception ex)
    {
        std::string str = ex.what();
    }
}

void RitmThread::stop()
{
    m_starter->stop();
}

void RitmThread::printString(const char * str)
{
    emit printStringSignal(tr(str));
}