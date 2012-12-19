#include "nproc.h"

NProc::NProc(QObject *parent) :
    QObject(parent)
{
}


QString NProc::proc()
{
    return m_proc;
}

void NProc::setProc(QString proc)
{
    m_proc = proc;
}


QString NProc::name()
{
    return m_name;
}

void NProc::setName(QString name)
{
    m_name = name;
}


//сериализация
QDomElement NProc::toXml(QDomDocument &doc)
{
    QDomElement proc = doc.createElement("proc");
    proc.setAttribute("name",name());

    QDomText text = doc.createTextNode(m_proc); proc.appendChild(text);

    return proc;
}

void NProc::fromXml(QDomElement &proc)
{
    setName( proc.attribute("name") );
    m_proc = proc.text();
}
