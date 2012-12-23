#ifndef QSPROXYMAN_H
#define QSPROXYMAN_H

#include <QObject>
#include "core/nframe.h"
#include "qsproxy.h"
#include <QScriptEngine>

class QSProxyMan : public QSProxy
{
    Q_OBJECT


public:
    explicit QSProxyMan(NFrame *man,QScriptEngine *engine,QObject *parent = 0);



signals:

public slots:


};

#endif // QSPROXYMAN_H
