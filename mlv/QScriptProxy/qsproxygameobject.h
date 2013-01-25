#ifndef QSPROXYGAMEOBJECT_H
#define QSPROXYGAMEOBJECT_H

//#include "mlv/mlv.h"
#include "qsproxy.h"

/**
  @class QSProxyGameObject
  Класс обертка для фрейма игровой объект для использования в скриптах
  */

class QSProxyGameObject : public QSProxy
{
    Q_OBJECT
public:
    explicit QSProxyGameObject(NFrame *frameGameObject,QScriptEngine *engine, MLV *mlv,QObject *parent = 0);



signals:

public slots:

};

#endif // QSPROXYGAMEOBJECT_H
