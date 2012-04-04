#ifndef NPROC_H
#define NPROC_H

#include <QObject>

//Процедура на qScript
class NProc : public QObject
{
    Q_OBJECT
public:
    explicit NProc(QObject *parent = 0);

    QString proc();
    void setProc(QString proc);

signals:

public slots:

private:
    QString m_proc;
};

#endif // NPROC_H
