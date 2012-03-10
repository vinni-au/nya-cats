#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include "domain.h"

class Variable : public QObject
{
    Q_OBJECT
public:
    explicit Variable(QString name="", QObject *parent = 0);

signals:

public slots:


public:
    QString name;
    bool derivable;//выводимая
    bool asking;    //запрашиваемая
    QString domain;
    QString question;

};

#endif // VARIABLE_H
