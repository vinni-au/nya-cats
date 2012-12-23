#include "qsproxy.h"

QSProxy::QSProxy(NFrame *frame,QScriptEngine *engine,QObject *parent) :
    QObject(parent),
    m_frame(frame),
    m_engine(engine)
{
}
