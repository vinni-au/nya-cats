#ifndef NPRODUCTION_H
#define NPRODUCTION_H

#include <QObject>
#include "rulemodel.h"

//Продукционная программка
//будет содержать модель для правил RuleModel

class NProduction : public QObject
{
    Q_OBJECT
public:
    explicit NProduction(QObject *parent = 0);

    RuleModel* getModel();

    //сериализация
    QDomElement toXml(QDomDocument &doc);
    void fromXml(QDomElement &production);


    QString name();
    void    setName(QString name);

    void clear();
signals:

public slots:

private:
    RuleModel m_ruleModel;

    QString m_name;
};

#endif // NPRODUCTION_H
