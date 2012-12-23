#include "qsproxycell.h"

QSProxyCell::QSProxyCell(NFrame *cell,QScriptEngine *engine,QObject *parent) :
    QSProxy(cell,engine,parent)
{
}

QScriptValue QSProxyCell::getMan()
{

    NFrame* man = 0;//dffd;

    //окружение скрипта
    QSProxyMan *proxyMan = new QSProxyMan(man, 0);

    QScriptValue objectMan = m_engine->newQObject(proxyMan);
    return objectMan;

//    engine->globalObject().setProperty("Me", objectMan);

//    //запускаем скрипт
//    QScriptValue result = engine->evaluate(script);
//    int res = result.toInt32();
//    if (result.isError());
//        qDebug() << "Script error:" << result.toString();
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
