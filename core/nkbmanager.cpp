#include "nkbmanager.h"

NKBManager::NKBManager(QObject *parent) :
    QObject(parent)
{
    dirty = false;
    file = NULL;

    Clear();

    connect(&domainModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));

    //контроль целостности модели
    //QObject::connect(domainModel,SIGNAL(sigDomainDeleted(QString)),varModel,SLOT(onDomainDeleted(QString)));
    //QObject::connect(domainModel,SIGNAL(sigDomainNameChanged(QString,QString)),varModel,SLOT(onDomainNameChanged(QString,QString)));

    //!QObject::connect(domainModel,SIGNAL(sigDomainValueChanged(QString,QString,QString)),ruleModel,SLOT(onDomainValueChanged(QString,QString,QString)));
    //!QObject::connect(domainModel,SIGNAL(sigDomainValueDeleted(QString,QString)),ruleModel,SLOT(onDomainValueDeleted(QString,QString)));
    //!QObject::connect(varModel,SIGNAL(sigVarNameChanged(QString,QString)),ruleModel,SLOT(onVarNameChanged(QString,QString)));

    //!QObject::connect(varModel,SIGNAL(sigVarDeleted(QString)),ruleModel,SLOT(onVarDeleted(QString)));
    //!QObject::connect(varModel,SIGNAL(sigVarDomainChanged(QString,QString,QString)),ruleModel,SLOT(onVarDomainChanged(QString,QString,QString)));

    //сигналы об ошибках валидации
    connect(&domainModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
    //connect(varModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
    //connect(ruleModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
}

//интерфейсные методы
///От диаграмм
void NKBManager::selectFrame(unsigned id)
{

}

void NKBManager::addFrame(QString name)
{

}

void NKBManager::deleteFrame(unsigned id)
{

}

//source id, destination id
void NKBManager::addIsa(unsigned sid, unsigned did)
{

}

void NKBManager::addApo(unsigned sid, unsigned did)
{

}

void NKBManager::deleteLink(unsigned sid, unsigned did)
{

}

//конец интерфейсные методы




bool
NKBManager::SaveAs()
{
    if(!this->isValid())
    {
        QMessageBox mb;
        mb.setText("В данных ошибка. Не сохранено.");
        mb.exec();
        return false;
    }
    QString esFile=QFileDialog::getSaveFileName(NULL,tr("Сохранить БЗ как.."),QDir::currentPath(),tr("NYA-CATS (*.nya)"),0);
    if(!esFile.isEmpty())
    {
        qDebug()<<esFile;
        if(!this->saveAs(esFile/*.append(".esxml")*/))
        {
            //если не сохранилось
            return false;
        }
        return true;
    }
    else
        return false;
}

bool
NKBManager::mayBeSave()
{
    qDebug()<<"NKBManager::mayBeSave";

    if(!this->saved())
    {
        qDebug()<<"ЭС не сохранена";
        QMessageBox mb;
        mb.setWindowTitle("Сохранить?");
        mb.setText(tr("Сохранить предыдущие действия?"));
        mb.setModal(true);
        mb.addButton(QMessageBox::Yes);
        mb.addButton(QMessageBox::No);
        mb.addButton(QMessageBox::Cancel);
        switch( mb.exec() )
        {
        case QMessageBox::Yes:
            qDebug()<<"Сохранить";
            if(this->isValid())
            {
                if(this->file!=NULL)
                {
                    qDebug()<<"Просто сохранить";
                    this->save();
                }
                else
                {
                    qDebug()<<"Сохранить как..";
                    if(!(SaveAs()))
                        return false;
                }
            }
            else
            {
                QMessageBox mbNotValid;
                mbNotValid.setText("В данных есть ошибка. Сохранение отменено.");
                mbNotValid.exec();
                qDebug()<<"Отмена";
                return false;
            }
            break;
        case QMessageBox::No:
            qDebug()<<"Не сохранять";
            break;
        case QMessageBox::Cancel:
            qDebug()<<"Отмена";
            return false;
            break;
        }
    }
    return true;
}

bool
NKBManager::areUsure(QString quest)
{
    QMessageBox mb;
    mb.setWindowTitle("А вы уверены?");
    mb.setText(quest);
    mb.addButton(QMessageBox::Ok);
    mb.addButton(QMessageBox::Cancel);
    mb.setDefaultButton(QMessageBox::Cancel);
    switch(mb.exec())
    {
    case QMessageBox::Ok:
        return true;
        break;
    case QMessageBox::Cancel:
        return false;
        break;
    }
    return false;
}
/////////////////////////из Datamodels





//DataModels::DataModels(QObject *parent):
//    QObject(parent)
//{
//    domainModel=new DomainModel();
//    varModel=new VarModel();
//    ruleModel=new RuleModel();
//    dirty = false;
//    file = NULL;

//    Clear();

//    connect(domainModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));
//    connect(varModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));
//    connect(ruleModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));

//    //контроль целостности модели
//    QObject::connect(domainModel,SIGNAL(sigDomainDeleted(QString)),varModel,SLOT(onDomainDeleted(QString)));
//    QObject::connect(domainModel,SIGNAL(sigDomainNameChanged(QString,QString)),varModel,SLOT(onDomainNameChanged(QString,QString)));

//    QObject::connect(domainModel,SIGNAL(sigDomainValueChanged(QString,QString,QString)),ruleModel,SLOT(onDomainValueChanged(QString,QString,QString)));
//    QObject::connect(domainModel,SIGNAL(sigDomainValueDeleted(QString,QString)),ruleModel,SLOT(onDomainValueDeleted(QString,QString)));
//    QObject::connect(varModel,SIGNAL(sigVarNameChanged(QString,QString)),ruleModel,SLOT(onVarNameChanged(QString,QString)));

//    QObject::connect(varModel,SIGNAL(sigVarDeleted(QString)),ruleModel,SLOT(onVarDeleted(QString)));
//    QObject::connect(varModel,SIGNAL(sigVarDomainChanged(QString,QString,QString)),ruleModel,SLOT(onVarDomainChanged(QString,QString,QString)));

//    //сигналы об ошибках валидации
//    connect(domainModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
//    connect(varModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
//    connect(ruleModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
//}


void
NKBManager::saveToXml(QTextStream &stream)
{
//    QDomDocument doc;

//    QDomElement es = doc.createElement("es");   doc.appendChild(es);

//    QDomElement domains = DataModels::instance()->domainModel->toXml(doc);   es.appendChild(domains);

//    QDomElement vars = DataModels::instance()->varModel->toXml(doc);   es.appendChild(vars);

//    QDomElement rules = DataModels::instance()->ruleModel->toXml(doc);   es.appendChild(rules);

//    doc.save(stream,4);

}
void
NKBManager::readFromXml(QFile &file)
{
//    QDomDocument doc;
//    if(!doc.setContent(&file))
//    {
//        qDebug()<<"DataModels::readFromXml: Не удалось считать файл: "<<file.fileName();
//        return;
//    }
//    QDomElement es = doc.documentElement();
//    QDomElement domains = es.firstChildElement("domains");
//    this->domainModel->fromXml(domains);
//    QDomElement vars = es.firstChildElement("vars");
//    this->varModel->fromXml(vars);
//    QDomElement rules = es.firstChildElement("rules");
//    this->ruleModel->fromXml(rules);
//    file.close();

//    dirty=false;
//    emit sigDataLoaded();
}

bool
NKBManager::saved()
{
    return !dirty;
}

void
NKBManager::save()
{
    if(!file)
    {
        qDebug("NKBManager::save() нет файла");
        return;
    }

    file->open(QIODevice::WriteOnly);
    QTextStream stream(file);
    saveToXml(stream);
    file->close();
    dirty = false;
}
bool
NKBManager::saveAs(QString filePath)
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
NKBManager::Open(QString filePath)
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
NKBManager::Clear()
{
    domainModel.setDomains(new QList<Domain*>());
//    varModel->setVariables(new QList<Variable*>());
//    ruleModel->setRules(new QList<Rule*>());

    file = NULL;
    dirty = false;

}
void
NKBManager::onDataChanged()
{
    qDebug()<<"NKBManager::onDataChanged()";
    dirty = true;
}

bool
NKBManager::isValid()
{
    //return domainModel->isValid()&&varModel->isValid()&&ruleModel->isValid();
    return 0;
}

bool NKBManager::reload()
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
