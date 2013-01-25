#ifndef NFASET_H
#define NFASET_H

#include <QObject>
#include <QtXml/QDomDocument>
#include <QVariant>


//Что:Фасет слота
//Что имеет:Имеет имя, значение
//значение может быть целого и строкового типа.
//Функции:
//Получать доступ к значению и к имени
//Сериализация в xml


/**
  @class NFaset
  Класс для представления фасета
  */
class NFaset : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName SCRIPTABLE true)//имя
    Q_PROPERTY(QVariant value READ value WRITE setValue SCRIPTABLE true)//значение

public:
    explicit NFaset(QObject *parent = 0);
    NFaset(QString name,QObject *parent =0);

    //name
    QString name();
    void setName(QString name);

    //value
    QString getStringValue();
    int getIntValue();
    void setStringValue(QString value);
    void setIntValue(int value);

    QVariant value();
    void setValue(QVariant val);

    //сериализация
    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &faset);

    void initFromFaset(NFaset *faset);
signals:
    void nameChanged();
    void valueChanged();
public slots:

private:
    QString m_name;         //имя фасета
    QVariant m_value;       //значение фасета

};

#endif // NFASET_H
