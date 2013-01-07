#ifndef QSPROXYMAN_H
#define QSPROXYMAN_H

#include <QObject>
#include "core/nframe.h"
#include "qsproxy.h"
#include <QScriptEngine>

class QSProxyMan : public QSProxy
{
    Q_OBJECT


    Q_PROPERTY(int Life READ Life WRITE setLife SCRIPTABLE true)

public:
    explicit QSProxyMan(NFrame *man,QScriptEngine *engine,QObject *parent = 0);


    int     Life();
    void    setLife(int Life);

signals:

public slots:


};

#endif // QSPROXYMAN_H
