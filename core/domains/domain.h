#ifndef DOMAIN_H
#define DOMAIN_H

#include <QObject>
#include <QVector>
/**
  @class Domain
  Класс для внутреннего представления домена
*/
class Domain : public QObject
{
    Q_OBJECT
public:
    ///Конструктор
    explicit Domain(QString name = "",QObject *parent = 0);

signals:

public slots:

public:
    ///Имя домена
    QString name;
    ///Список значений домена
    QVector<QString> values;
};

#endif // DOMAIN_H
