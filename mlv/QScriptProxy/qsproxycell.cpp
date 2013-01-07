#include "qsproxycell.h"

QSProxyCell::QSProxyCell(NFrame *cell,QScriptEngine *engine,QObject *parent) :
    QSProxy(cell,engine,parent)
{
}

QScriptValue QSProxyCell::getMan()
{
    NSlot* slot = m_frame->getSlotByName(SYSSTR_SLOTNAME_GAMEITEM);
    NFrame* man = (NFrame*)slot->getFasetByName("value")->value().toLongLong();
    //окружение скрипта
    QSProxyMan *proxyMan = new QSProxyMan(man, 0);

    QScriptValue objectMan = m_engine->newQObject(proxyMan);
    return objectMan;
}

bool QSProxyCell::hasGameObject(QString objName)
{
    NSlot* slot = m_frame->getSlotByName(SYSSTR_SLOTNAME_GAMEITEM);
    NFrame* gameObj = (NFrame*)slot->getFasetByName("value")->value().toLongLong();
    if(!gameObj)
        return false;
    NSlot* name = gameObj->getSlotByName("name");

    if(!name)
        return false;

    QString str_name = name->getFasetByName("default_value")->value().toString();

    return str_name==objName ? true : false;
}



QScriptValue QSProxyCell::getFood()
{
    return QScriptValue();
}

void QSProxyCell::Clear()
{

}

int QSProxyCell::X()
{
    QVariant xv = m_frame->GetSlotFaset("x","value")->value();
    int x = xv.toInt();
    return x;//m_frame->GetSlotFaset("x","value")->value().toInt();
}

void QSProxyCell::setX(int X)
{
    m_frame->GetSlotFaset("x","value")->setValue(X);
}

int QSProxyCell::Y()
{
    return m_frame->GetSlotFaset("y","value")->value().toInt();
}

void QSProxyCell::setY(int Y)
{
    m_frame->GetSlotFaset("y","value")->setValue(Y);
}
