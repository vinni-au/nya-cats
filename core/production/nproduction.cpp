#include "nproduction.h"

NProduction::NProduction(QObject *parent) :
    QObject(parent)
{

}

RuleModel* NProduction::getModel()
{
    return &m_ruleModel;
}


QDomElement NProduction::toXml(QDomDocument &doc)
{
    QDomElement productionEl = doc.createElement("production");
    productionEl.setAttribute("name",m_name);

    QDomElement rulesEl = this->m_ruleModel.toXml(doc);
    productionEl.appendChild(rulesEl);
    return productionEl;
}

void NProduction::fromXml(QDomElement &production)
{
    m_name = production.attribute("name");

    QDomElement rulesEl = production.firstChild().toElement();

    this->m_ruleModel.fromXml(rulesEl);
}


