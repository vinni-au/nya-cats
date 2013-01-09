#ifndef QSPROXY_H
#define QSPROXY_H

#include <QObject>
#include "core/nframe.h"
#include <QScriptEngine>
#include "mlv/mlv.h"

//class MLV;

class QSProxy : public QObject
{
    Q_OBJECT
public:
    explicit QSProxy(NFrame *frame,QScriptEngine *engine, MLV *mlv, QObject *parent = 0);


    Q_INVOKABLE QVariant getSlotValue(QString slotName);
    Q_INVOKABLE void setSlotValue(QString slotName,QVariant value);
signals:

public slots:

protected:
    NFrame  *m_frame;
    QScriptEngine *m_engine;
    MLV *m_mlv;

};

#endif // QSPROXY_H
