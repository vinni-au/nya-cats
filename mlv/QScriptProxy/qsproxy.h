#ifndef QSPROXY_H
#define QSPROXY_H

#include <QObject>
#include "core/nframe.h"
#include <QScriptEngine>

class QSProxy : public QObject
{
    Q_OBJECT
public:
    explicit QSProxy(NFrame *frame,QScriptEngine *engine,QObject *parent = 0);

signals:

public slots:

protected:
    NFrame  *m_frame;
    QScriptEngine *m_engine;

};

#endif // QSPROXY_H
