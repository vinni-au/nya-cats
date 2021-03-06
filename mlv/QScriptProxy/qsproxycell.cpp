#include "qsproxycell.h"

QSProxyCell::QSProxyCell(NFrame *cell,QScriptEngine *engine, MLV *mlv,QObject *parent) :
    QSProxy(cell,engine,mlv,parent)
{
}

QScriptValue QSProxyCell::getMan()
{
    NSlot* slot = m_frame->getSlotByName(SYSSTR_SLOTNAME_GAMEITEM);
    NFrame* man = (NFrame*)slot->getFasetByName("value")->value().toLongLong();
    //окружение скрипта
    QSProxyMan *proxyMan = new QSProxyMan(man,m_engine,m_mlv, this);

    QScriptValue objectMan = m_engine->newQObject(proxyMan);
    return objectMan;
}

QScriptValue QSProxyCell::getGameObject()
{
    NSlot* slot = m_frame->getSlotByName(SYSSTR_SLOTNAME_GAMEITEM);
    NFrame* go = (NFrame*)slot->getFasetByName("value")->value().toLongLong();
    //окружение скрипта
    QSProxyGameObject *proxyGO = new QSProxyGameObject(go,m_engine,m_mlv, this);

    QScriptValue objectGO = m_engine->newQObject(proxyGO);
    return objectGO;
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
{//сделать через млв
    m_mlv->ClearCell(m_frame);
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

NFrame* QSProxyCell::getFrame()
{
    return m_frame;
}
