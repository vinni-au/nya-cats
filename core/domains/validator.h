#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QObject>
#include <QStringList>


class Validator : public QObject
{
    Q_OBJECT
public:
    explicit Validator(QObject *parent = 0);

public:
    static bool NoCopies(QStringList& strList);
    static bool NoCopies(QList<QString> strList);

signals:

public slots:

};

#endif // VALIDATOR_H
