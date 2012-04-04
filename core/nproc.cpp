#include "nproc.h"

NProc::NProc(QObject *parent) :
    QObject(parent)
{
}


QString NProc::proc()
{
    return m_proc;
}

void NProc::setProc(QString proc)
{
    m_proc = proc;
}
