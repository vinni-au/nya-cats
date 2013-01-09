#include "qsproxy.h"

QSProxy::QSProxy(NFrame *frame,QScriptEngine *engine, MLV *mlv,QObject *parent) :
    QObject(parent),
    m_frame(frame),
    m_engine(engine),
    m_mlv(mlv)
{
}

QVariant QSProxy::getSlotValue(QString slotName)
{
    NSlot* slot = m_frame->getSlotByName(slotName);
    if(!slot)
        return QVariant();
    NFaset* faset = slot->getFasetByName("value");
    if(!faset)
        return QVariant();
    return faset->value();
}

void QSProxy::setSlotValue(QString slotName,QVariant value)
{
    NSlot* slot = m_frame->getSlotByName(slotName);
    if(!slot)
        return;
    NFaset* faset = slot->getFasetByName("value");
    if(!faset)
        return;
    faset->setValue(value);
}
