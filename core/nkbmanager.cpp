#include "nkbmanager.h"

NKBManager::NKBManager(QObject *parent) :
    QObject(parent)
{
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
//!!    if(!DataModels::instance()->isValid())
//    {
//        QMessageBox mb;
//        mb.setText("В данных ошибка. Не сохранено.");
//        mb.exec();
//        return false;
//    }
//    QString esFile=QFileDialog::getSaveFileName(NULL,tr("Сохранить ЭС как.."),QDir::currentPath(),tr("Expert system (*.esxml)"),0);
//    if(!esFile.isEmpty())
//    {
//        qDebug()<<esFile;
//        if(!DataModels::instance()->saveAs(esFile/*.append(".esxml")*/))
//        {
//            //если не сохранилось
//            return false;
//        }
//        return true;
//    }
//    else
//        return false;
}

bool
NKBManager::mayBeSave()
{
//    qDebug()<<"ESmain::maybeSave()";

//    if(!DataModels::instance()->saved())
//    {
//        qDebug()<<"ЭС не сохранена";
//        QMessageBox mb;
//        mb.setWindowTitle("Сохранить?");
//        mb.setText(tr("Сохранить предыдущие действия?"));
//        mb.setModal(true);
//        mb.addButton(QMessageBox::Yes);
//        mb.addButton(QMessageBox::No);
//        mb.addButton(QMessageBox::Cancel);
//        switch( mb.exec() )
//        {
//        case QMessageBox::Yes:
//            qDebug()<<"Сохранить";
//            if(DataModels::instance()->isValid())
//            {
//                if(DataModels::instance()->file!=NULL)
//                {
//                    qDebug()<<"Просто сохранить";
//                    DataModels::instance()->save();
//                }
//                else
//                {
//                    qDebug()<<"Сохранить как..";
//                    if(!(Saver::SaveAs()))
//                        return false;
//                }
//            }
//            else
//            {
//                QMessageBox mbNotValid;
//                mbNotValid.setText("В данных есть ошибка. Сохранение отменено.");
//                mbNotValid.exec();
//                qDebug()<<"Отмена";
//                return false;
//            }
//            break;
//        case QMessageBox::No:
//            qDebug()<<"Не сохранять";
//            break;
//        case QMessageBox::Cancel:
//            qDebug()<<"Отмена";
//            return false;
//            break;
//        }
//    }
//    return true;
}

bool
NKBManager::areUsure(QString quest)
{
//    QMessageBox mb;
//    mb.setWindowTitle("А вы уверены?");
//    mb.setText(quest);
//    mb.addButton(QMessageBox::Ok);
//    mb.addButton(QMessageBox::Cancel);
//    mb.setDefaultButton(QMessageBox::Cancel);
//    switch(mb.exec())
//    {
//    case QMessageBox::Ok:
//        return true;
//        break;
//    case QMessageBox::Cancel:
//        return false;
//        break;
//    }
//    return false;
}
