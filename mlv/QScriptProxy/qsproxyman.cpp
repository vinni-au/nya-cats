#include "qsproxyman.h"

QSProxyMan::QSProxyMan(NFrame *man,QScriptEngine *engine,QObject *parent) :
    QSProxy(man,engine,parent)
{
}


int     QSProxyMan::Life()
{
    //m_frame->getSlotByName("Уровень жизни")->getFasetByName("value")->value();

    NSlot* slot = m_frame->getSlotByName("Уровень жизни");
    NFaset* faset = slot->getFasetByName("value");
    int val = faset->value().toInt();
    return val;
}

void    QSProxyMan::setLife(int Life)
{
    m_frame->getSlotByName("Уровень жизни")->getFasetByName("value")->setValue(Life);
}


