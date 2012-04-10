#include "nkbmanager.h"

NKBManager::NKBManager(QObject *parent) :
    QObject(parent)
{
    dirty = false;
    file = NULL;

    Clear();

    connect(&m_domainModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));

    //контроль целостности модели
    //QObject::connect(domainModel,SIGNAL(sigDomainDeleted(QString)),varModel,SLOT(onDomainDeleted(QString)));
    //QObject::connect(domainModel,SIGNAL(sigDomainNameChanged(QString,QString)),varModel,SLOT(onDomainNameChanged(QString,QString)));

    //!QObject::connect(domainModel,SIGNAL(sigDomainValueChanged(QString,QString,QString)),ruleModel,SLOT(onDomainValueChanged(QString,QString,QString)));
    //!QObject::connect(domainModel,SIGNAL(sigDomainValueDeleted(QString,QString)),ruleModel,SLOT(onDomainValueDeleted(QString,QString)));
    //!QObject::connect(varModel,SIGNAL(sigVarNameChanged(QString,QString)),ruleModel,SLOT(onVarNameChanged(QString,QString)));

    //!QObject::connect(varModel,SIGNAL(sigVarDeleted(QString)),ruleModel,SLOT(onVarDeleted(QString)));
    //!QObject::connect(varModel,SIGNAL(sigVarDomainChanged(QString,QString,QString)),ruleModel,SLOT(onVarDomainChanged(QString,QString,QString)));

    //сигналы об ошибках валидации
    connect(&m_domainModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
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
    QDomDocument doc;
    QDomElement kbEl = doc.createElement("kb");   doc.appendChild(kbEl);
    QDomElement domainsEl = m_domainModel.toXml(doc);   kbEl.appendChild(domainsEl);
    QDomElement framesEl = doc.createElement("frames"); kbEl.appendChild(framesEl);

    //Фреймы в xml
    NFrame *frame;
    foreach(frame,m_frames)
    {
        QDomElement frameEl = frame->toXml(doc);
        framesEl.appendChild(frameEl);
    }
    //процедуры
    QDomElement procsEl = doc.createElement("procs"); kbEl.appendChild(procsEl);
    NProc *proc;
    foreach(proc,m_procs)
    {
        QDomElement procEl = proc->toXml(doc);
        procsEl.appendChild(procEl);
    }
    //продукции
    QDomElement productionsEl = doc.createElement("productions"); kbEl.appendChild(productionsEl);
    NProduction *production;
    foreach(production,m_productions)
    {
        QDomElement productionEl = production->toXml(doc);
        productionsEl.appendChild(productionEl);
    }

    doc.save(stream,4);

}
void
NKBManager::readFromXml(QFile &file)
{
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        qDebug()<<"NKBManager::readFromXml: Не удалось считать файл: "<<file.fileName();
        return;
    }

    QDomElement kbEl = doc.documentElement();
    //домены
    QDomElement domainsEl = kbEl.firstChildElement("domains");
    m_domainModel.fromXml(domainsEl);
    //фреймы
    QDomElement framesEl = kbEl.firstChildElement("frames");
    QDomElement frameEl = framesEl.firstChild().toElement();
    while(!frameEl.isNull())
    {
        NFrame *frame = new NFrame();
        frame->fromXml(frameEl);
        m_frames.append(frame);
        frameEl = frameEl.nextSibling().toElement();
    }
    //процедуры
    QDomElement procsEl = kbEl.firstChildElement("procs");
    QDomElement procEl = procsEl.firstChild().toElement();
    while(!procEl.isNull())
    {
        NProc *proc = new NProc();
        proc->fromXml(procEl);
        m_procs.append(proc);
        procEl = procEl.nextSibling().toElement();
    }
    //продукции
    QDomElement productionsEl = kbEl.firstChildElement("productions");
    QDomElement prodictionEl = productionsEl.firstChild().toElement();
    while(!prodictionEl.isNull())
    {
        NProduction *production = new NProduction();
        production->fromXml(prodictionEl);
        m_productions.append(production);
        prodictionEl = prodictionEl.nextSibling().toElement();
    }
    file.close();
    dirty=false;
    emit sigDataLoaded();
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
void//todo
NKBManager::Clear()
{
    m_domainModel.setDomains(new QList<Domain*>());
    m_frames.clear();
    m_productions.clear();
    m_procs.clear();

    file = NULL;
    dirty = false;
}
void
NKBManager::onDataChanged()
{
    qDebug()<<"NKBManager::onDataChanged()";
    dirty = true;
}

bool//todo
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
