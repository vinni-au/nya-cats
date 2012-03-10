#include "datamodels.h"

DataModels* DataModels::theSingleInstance=NULL;

DataModels::DataModels(QObject *parent):
    QObject(parent)
{
    domainModel=new DomainModel();
    varModel=new VarModel();
    ruleModel=new RuleModel();
    dirty = false;
    file = NULL;

    Clear();

    connect(domainModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));
    connect(varModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));
    connect(ruleModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));

    //контроль целостности модели
    QObject::connect(domainModel,SIGNAL(sigDomainDeleted(QString)),varModel,SLOT(onDomainDeleted(QString)));
    QObject::connect(domainModel,SIGNAL(sigDomainNameChanged(QString,QString)),varModel,SLOT(onDomainNameChanged(QString,QString)));

    QObject::connect(domainModel,SIGNAL(sigDomainValueChanged(QString,QString,QString)),ruleModel,SLOT(onDomainValueChanged(QString,QString,QString)));
    QObject::connect(domainModel,SIGNAL(sigDomainValueDeleted(QString,QString)),ruleModel,SLOT(onDomainValueDeleted(QString,QString)));
    QObject::connect(varModel,SIGNAL(sigVarNameChanged(QString,QString)),ruleModel,SLOT(onVarNameChanged(QString,QString)));

    QObject::connect(varModel,SIGNAL(sigVarDeleted(QString)),ruleModel,SLOT(onVarDeleted(QString)));
    QObject::connect(varModel,SIGNAL(sigVarDomainChanged(QString,QString,QString)),ruleModel,SLOT(onVarDomainChanged(QString,QString,QString)));

    //сигналы об ошибках валидации
    connect(domainModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
    connect(varModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
    connect(ruleModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
}
DataModels::~DataModels()
{
    delete domainModel;
    delete varModel;
    delete ruleModel;
}

void
DataModels::saveToXml(QTextStream &stream)
{
    QDomDocument doc;

    QDomElement es = doc.createElement("es");   doc.appendChild(es);

    QDomElement domains = DataModels::instance()->domainModel->toXml(doc);   es.appendChild(domains);

    QDomElement vars = DataModels::instance()->varModel->toXml(doc);   es.appendChild(vars);

    QDomElement rules = DataModels::instance()->ruleModel->toXml(doc);   es.appendChild(rules);

    doc.save(stream,4);

}
void
DataModels::readFromXml(QFile &file)
{
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        qDebug()<<"DataModels::readFromXml: Не удалось считать файл: "<<file.fileName();
        return;
    }
    QDomElement es = doc.documentElement();
    QDomElement domains = es.firstChildElement("domains");
    this->domainModel->fromXml(domains);
    QDomElement vars = es.firstChildElement("vars");
    this->varModel->fromXml(vars);
    QDomElement rules = es.firstChildElement("rules");
    this->ruleModel->fromXml(rules);
    file.close();

    dirty=false;
    emit sigDataLoaded();
}

bool
DataModels::saved()
{
    return !dirty;
}

void
DataModels::save()
{
    if(!file)
    {
        qDebug("DataModels::save() нет файла");
        return;
    }

    file->open(QIODevice::WriteOnly);
    QTextStream stream(file);
    saveToXml(stream);
    file->close();
    dirty = false;
}
bool
DataModels::saveAs(QString filePath)
{
    qDebug()<<"DataModels::saveAs ::"<<filePath;
    QFile *newFile = new QFile(filePath);
//    if(newFile->exists())//такой файл уже существует
//    {
//        qDebug()<<"Такой файл уже существует";
//        return false;
//    }
    this->file = newFile;
    save();
    return true;
}

bool
DataModels::Open(QString filePath)
{
    qDebug()<<"DataModels::Open ::"<<filePath;
    QFile *newFile = new QFile(filePath);
    if(!newFile->exists())//если нет такого файла
    {
        qDebug()<<"Нет такого файла";
        return false;
    }
    this->file = newFile;
    file->open(QIODevice::ReadOnly);
    readFromXml(*newFile);
    file->close();
    dirty=false;
    return true;
}
void
DataModels::Clear()
{
    domainModel->setDomains(new QList<Domain*>());
    varModel->setVariables(new QList<Variable*>());
    ruleModel->setRules(new QList<Rule*>());

    file = NULL;
    dirty = false;

}
void
DataModels::onDataChanged()
{
    qDebug()<<"DataModels::onDataChanged()";
    dirty = true;
}

bool
DataModels::isValid()
{
    return domainModel->isValid()&&varModel->isValid()&&ruleModel->isValid();
}

bool DataModels::reload()
{
    if(this->file!=NULL)
    {
        this->readFromXml(*(this->file));
    }
    else
    {
        this->Clear();

    }
    dirty = false;
    return true;
}
