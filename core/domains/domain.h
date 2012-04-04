#ifndef DOMAIN_H
#define DOMAIN_H

#include <QObject>
#include <QVector>

class Domain : public QObject
{
    Q_OBJECT
public:
    explicit Domain(QString name = "",QObject *parent = 0);

signals:

public slots:

public:
    QString name;
    QVector<QString> values;
};

#endif // DOMAIN_H
