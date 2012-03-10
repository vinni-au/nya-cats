#include "mycombobox.h"
#include <QDebug>

MyComboBox::MyComboBox(QWidget *parent) :
    QComboBox(parent)
{
    this->setEditable(true);
    this->setInsertPolicy(QComboBox::InsertAtBottom);
    currentInx=previousInx=0;
    connect(this,SIGNAL(currentIndexChanged(int)),SLOT(onCurrentIndexChanged(int)));
}

void
MyComboBox::onCurrentIndexChanged(int index)
{
    previousInx=currentInx;
    currentInx = index;
}

void MyComboBox::selectPrevious()
{
    this->setCurrentIndex(previousInx);
}

void
MyComboBox::keyPressEvent(QKeyEvent *e)
{
    qDebug()<<"MyComboBox::keyPressEvent(QKeyEvent *e)"<<e->key();
    if(e->key() == 16777220)//если нажали ентер
    {
        if(this->currentText().trimmed().isEmpty())
            return;
        QString newValue = this->currentText();

        bool found = false;
        int sameIndex = -1;

        for(int i=0; i<this->count();++i)
        {
            if(this->itemText(i).trimmed().compare(newValue.trimmed(),Qt::CaseInsensitive)==0)//совпадают
            {
                found = true;
                sameIndex = i;
                break;
            }
        }

        if(!found)
        {
            qDebug()<<"MyComboBox::keyPressEvent: emit sigNewValueEntered: "<<newValue;
            emit sigNewValueEntered(newValue);
        }
        else//селектим итем
        {
            qDebug()<<"Select "<<this->itemText(sameIndex);
            this->setCurrentIndex(sameIndex);  
        }
        e->accept();
    }
    else
        QComboBox::keyPressEvent(e);
}
