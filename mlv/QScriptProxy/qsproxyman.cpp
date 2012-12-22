#include "qsproxyman.h"

QSProxyMan::QSProxyMan(NFrame *man,QObject *parent) :
    QObject(parent),
    m_frame(man)
{
}


int QSProxyMan::X()
{
    return 0;//m_frame->GetSlotFaset("x","value")->value().toInt();
}

void QSProxyMan::setX(int X)
{

}
