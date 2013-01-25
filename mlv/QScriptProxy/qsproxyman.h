#ifndef QSPROXYMAN_H
#define QSPROXYMAN_H

#include <QObject>
#include "core/nframe.h"
#include "qsproxy.h"
#include <QScriptEngine>
//#include "mlv/mlv.h"

/**
  @class QSProxyMan
  Класс обертка для фрейма Персонаж для использования в скриптах
  */

class QSProxyMan : public QSProxy
{
    Q_OBJECT


    Q_PROPERTY(int Life READ Life WRITE setLife SCRIPTABLE true)

public:
    explicit QSProxyMan(NFrame *man,QScriptEngine *engine, MLV *mlv,QObject *parent = 0);


    int     Life();
    void    setLife(int Life);

signals:

public slots:


};

#endif // QSPROXYMAN_H
