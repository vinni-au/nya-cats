#ifndef QSPROXYGAMEOBJECT_H
#define QSPROXYGAMEOBJECT_H

#include "mlv/QScriptProxy/qsproxy.h"

class QSProxyGameObject : public QSProxy
{
    Q_OBJECT
public:
    explicit QSProxyGameObject(NFrame *frameGameObject,QScriptEngine *engine,QObject *parent = 0);



signals:

public slots:

};

#endif // QSPROXYGAMEOBJECT_H
