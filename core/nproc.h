#ifndef NPROC_H
#define NPROC_H

#include <QObject>
#include <QDomDocument>

//Процедура на qScript
class NProc : public QObject
{
    Q_OBJECT
public:
    explicit NProc(QObject *parent = 0);

    QString proc();
    void setProc(QString proc);

    QString name();
    void setName(QString name);


    //сериализация
    QDomElement toXml(QDomDocument &doc);
    void fromXml(QDomElement &proc);
signals:

public slots:

private:
    QString m_proc;

    QString m_name;
};

#endif // NPROC_H