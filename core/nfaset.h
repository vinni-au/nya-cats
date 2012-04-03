#ifndef NFASET_H
#define NFASET_H

#include <QObject>
#include <QtXml/QDomDocument>


//Что:Фасет слота
//Что имеет:Имеет имя, значение
//значение может быть целого и строкового типа.
//Функции:
//Получать доступ к значению и к имени
//Сериализация в xml


class NFaset : public QObject
{
    enum FASET_TYPE
    {
        type_int,type_string
    };


    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName SCRIPTABLE true)//имя

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

    //type
    FASET_TYPE type();
    //void setType(FASET_TYPE type);



    //сериализация
    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &faset);

signals:
    void nameChanged();
    void valueChanged();
public slots:

private:
    FASET_TYPE m_type;

    QString m_name;
    //value будет фишечка))) множественные значения фасета
    QString m_stringValue;
    int m_intValue;

};

#endif // NFASET_H
