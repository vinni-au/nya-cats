#include "sloteditorwnd.h"
#include "ui_sloteditorwnd.h"

SlotEditorWnd::SlotEditorWnd(QModelIndex slotIndex,NKBManager *kbManager,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SlotEditorWnd)
{
    ui->setupUi(this);
    m_slotIndex = slotIndex;
    m_kbManager = kbManager;

    NFramenetModel* model = (NFramenetModel*)( m_slotIndex.model());

    ui->lineEdit->setText( model->getSlotFasetValue(slotIndex,"name").toString()   );

    //тип слота
    ui->cmbSlotType->addItem("string");
    ui->cmbSlotType->addItem("int");
    ui->cmbSlotType->addItem("frame");
    ui->cmbSlotType->addItem("domain");


    QString slotType = model->getSlotFasetValue(slotIndex,"slot_type").toString() ;
    int slotTypeIndex = ui->cmbSlotType->findText(slotType);
    if(slotTypeIndex==-1)
        slotTypeIndex=3;
    ui->cmbSlotType->setCurrentIndex(slotTypeIndex);


    //домен
    ui->cmbSlotDomain->setModel( kbManager->getDomainModel()  );

    QString slotDomain = model->getSlotFasetValue(slotIndex,"slot_type").toString() ;
    int slotDomainIndex = ui->cmbSlotDomain->findText(slotDomain);
    ui->cmbSlotDomain->setCurrentIndex(slotDomainIndex);
    //значение


    //значение по умолчанию
    QString slotDefValue = model->getSlotFasetValue(slotIndex,"default_value").toString() ;
    if(ui->cmbSlotType->currentText()=="string")
    {
        ui->cmbDefaultValue->addItem( slotDefValue  );
        int inx=ui->cmbDefaultValue->findText(slotDefValue);
        ui->cmbDefaultValue->setCurrentIndex(inx);
    }
    else if(ui->cmbSlotType->currentText()=="frame" || ui->cmbSlotType->currentText()=="domain")
    {
        int inx=ui->cmbDefaultValue->findText(slotDefValue);
        ui->cmbDefaultValue->setCurrentIndex(inx);
    }
    else if(ui->cmbSlotType->currentText()=="int")
    {
        ui->cmbDefaultValue->addItem( slotDefValue  );
        int inx=ui->cmbDefaultValue->findText(slotDefValue);
        ui->cmbDefaultValue->setCurrentIndex(inx);
    }
    //наследование

    ui->cmbInheritance->addItem("same");
    ui->cmbInheritance->addItem("override");

    QString slotInheritance = model->getSlotFasetValue(slotIndex,"inheritance").toString() ;
    int slotInheritanceIndex = ui->cmbInheritance->findText(slotInheritance);
    ui->cmbInheritance->setCurrentIndex(slotInheritanceIndex);
    //тип маркера
    //ui->cmbMarkerType->addItem("domain");
    ui->cmbMarkerType->addItem("production");
    ui->cmbMarkerType->addItem("procedure");

    QString slotMarkerType = model->getSlotFasetValue(slotIndex,"marker_type").toString() ;
    int slotMarkerTypeIndex = ui->cmbMarkerType->findText(slotMarkerType);
    ui->cmbMarkerType->setCurrentIndex(slotMarkerTypeIndex);
    //значение маркера
    QString slotMarkerValue = model->getSlotFasetValue(slotIndex,"marker").toString() ;
    ui->cmbMarkerValue->setCurrentIndex( ui->cmbMarkerValue->findText( slotMarkerValue ) );

    //начальная позиция курсока
    ui->lineEdit->setFocus();

    //пока убрали наследование
    ui->lblInheritance->setVisible(false);
    ui->cmbInheritance->setVisible(false);
}

SlotEditorWnd::~SlotEditorWnd()
{
    delete ui;
}



void SlotEditorWnd::on_cmbSlotType_currentIndexChanged(int index)
{
    QString slotType = ui->cmbSlotType->currentText();
    qDebug()<<"SlotEditorWnd::on_cmbSlotType_currentIndexChanged "<<slotType;

    if(slotType.isEmpty())
        return;

    ui->lineEdit->setEnabled(true);
    ui->cmbSlotType->setEnabled(true);
    ui->cmbSlotDomain->setEnabled(true);
    ui->cmbDefaultValue->setEnabled(true);
    ui->cmbInheritance->setEnabled(true);
    ui->cmbMarkerType->setEnabled(true);
    ui->cmbMarkerValue->setEnabled(true);


    if(slotType == "string")
    {
        ui->cmbSlotDomain->setEnabled(false);
        ui->cmbSlotDomain->setModel(new QStringListModel());//опустошаем комбик доменов
        ui->cmbDefaultValue->setModel(new QStringListModel());
    }
    else if(slotType == "int")
    {
        ui->cmbSlotDomain->setEnabled(false);
        ui->cmbSlotDomain->setModel(new QStringListModel());//опустошаем комбик доменов
        ui->cmbDefaultValue->setModel(new QStringListModel());


        ui->cmbDefaultValue->addItem("0");
        ui->cmbDefaultValue->setCurrentIndex(0);

        ui->cmbMarkerType->clear();

        ui->cmbMarkerType->addItem("production");
        ui->cmbMarkerType->addItem("procedure");
    }
    else if(slotType == "frame")
    {
        ui->cmbSlotDomain->setEnabled(false);
        ui->cmbSlotDomain->setModel(new QStringListModel());//опустошаем комбик доменов

        QString frameName = m_kbManager->getFrameNetModel()->getFrameNameByIndex(m_slotIndex);
        QString slotName = m_kbManager->getFrameNetModel()->getSlotFasetValue(m_slotIndex,"name").toString();
        QStringList framesList=m_kbManager->getFilteredFrameList(frameName,slotName);

        QStringListModel* model = new QStringListModel(framesList);

        ui->cmbDefaultValue->setModel(model);
        ui->cmbDefaultValue->setRootModelIndex(QModelIndex());


    }
    else if(slotType == "domain")
    {
        ui->cmbSlotDomain->setModel( m_kbManager->getDomainModel()  );
    }
}

void SlotEditorWnd::on_btnEditMarker_clicked()
{
    if(ui->cmbMarkerType->currentText() == "domain")
    {

    }
    else if(ui->cmbMarkerType->currentText() == "production")
    {
        qDebug()<<"Добавление продукции";
        QString prodName;
        bool newProd = true;
        NProduction *production = NULL;

        if(ui->cmbMarkerValue->currentText().trimmed().isEmpty())
        {//добавить
            bool ok;
            QString name = QInputDialog::getText(this, "Создать продукии", "Введите имя продукционной программы:", QLineEdit::Normal, QString(), &ok);
            if (ok && !name.isEmpty())
            {
                if(!(m_kbManager->productionExists(name)))
                {
                    production = new NProduction();
                    production->setName(name);
                    newProd = true;
                }
                else
                {
                    QMessageBox::information(this,"","Прод. программа с таким именем уже существует",QMessageBox::Ok);
                    return;
                }
            }
            else
            {
                return;
            }
        }
        else
        {//изменить
            prodName = ui->cmbMarkerValue->currentText().trimmed();
            production = m_kbManager->getProduction(prodName);
            if(!production)
            {
                QMessageBox::information(this,"","Не удалось найти продукционную программу",QMessageBox::Ok);
                return;
            }
            newProd = false;
        }


        RulesWnd *rWnd = new RulesWnd(m_kbManager,production,newProd,m_slotIndex,ui->cmbSlotDomain->currentText(),this);
        rWnd->setWindowModality(Qt::WindowModal);
        QObject::connect(rWnd,SIGNAL(sigProductionAdded(NProduction*,bool)),this,SLOT(onProductionAdded(NProduction*,bool)));
        rWnd->show();
    }
    else if(ui->cmbMarkerType->currentText() == "procedure")
    {

    }
}


void SlotEditorWnd::onProductionAdded(NProduction *production, bool newProd)
{
    //QMessageBox::information(this,"SlotEditorWnd::onProductionAdded","Опачки, продукция добавлена.",QMessageBox::NoButton);
    if(m_kbManager->productionExists(production->name()))
    {
        //изменение
    }
    else
    {
        //добавление
        m_kbManager->addProduction(production);
    }
    ui->cmbMarkerValue->addItem(production->name());
    ui->cmbMarkerValue->setCurrentIndex( ui->cmbMarkerValue->findText(production->name()) );
    m_kbManager->mayBeSave();//????????????????????????????????????????????????????????????????????????????????????
}

void SlotEditorWnd::on_cmbMarkerType_currentIndexChanged(const QString &arg1)
{
    QString markerType = arg1;
    if(markerType == "domain")
    {

    }
    else if(markerType == "production")
    {
        ui->cmbMarkerValue->clear();
        QStringList names = m_kbManager->getProductionNames();
        ui->cmbMarkerValue->addItems(names);
        ui->cmbMarkerValue->addItem("");

    }
    else if(markerType == "procedure")
    {
        ui->cmbMarkerValue->clear();
        QStringList names = m_kbManager->getProceduresNames();
        ui->cmbMarkerValue->addItems(names);
        ui->cmbMarkerValue->addItem("");
    }
}

void SlotEditorWnd::on_cmbSlotDomain_currentIndexChanged(const QString &arg1)
{
    if(!ui->cmbSlotDomain->isEnabled())
        return;
    QString domain = arg1;
    QModelIndex domainIndex = m_kbManager->getDomainModel()->indexByName(domain);
    if(domainIndex.isValid())
    {
        ui->cmbDefaultValue->setModel(m_kbManager->getDomainModel());
        ui->cmbDefaultValue->setRootModelIndex(domainIndex);
        ui->cmbDefaultValue->setCurrentIndex(0);
    }
    else
    {
        ui->cmbDefaultValue->setModel(new QStringListModel());
        qDebug()<<"ui->cmbDefaultValue->setModel(new QStringListModel());";
    }
}

bool SlotEditorWnd::dataIsValid()
{
    if(ui->lineEdit->text().trimmed().isEmpty())
    {
        return false;
    }
    if(ui->cmbSlotType->currentText()=="int")
    {
        QVariant defVal(ui->cmbDefaultValue->currentText());
        bool ok;
        defVal.toInt(&ok);
        if(defVal.toString().isEmpty() || !ok )
        {
            return false;
        }
    }
    return true;
}

void SlotEditorWnd::on_btnOK_clicked()
{
    if(!dataIsValid())
    {
        QMessageBox::information(this,"Ошибка","Ошибка в данных",QMessageBox::Ok);
        return;
    }
    NFramenetModel* model = m_kbManager->getFrameNetModel();
    model->setSlotFasetValue(m_slotIndex,"name",ui->lineEdit->text().trimmed());//имя
    if(ui->cmbSlotType->currentText() == "domain")
    {
        model->setSlotFasetValue(m_slotIndex,"slot_type",ui->cmbSlotDomain->currentText());//тип
    }
    else
    {
        model->setSlotFasetValue(m_slotIndex,"slot_type",ui->cmbSlotType->currentText());
    }
    model->setSlotFasetValue(m_slotIndex,"default_value",ui->cmbDefaultValue->currentText());//задание отсутствия
    model->setSlotFasetValue(m_slotIndex,"inheritance",ui->cmbInheritance->currentText());//наследование
    model->setSlotFasetValue(m_slotIndex,"marker_type",ui->cmbMarkerType->currentText());//тип маркера
    model->setSlotFasetValue(m_slotIndex,"marker",ui->cmbMarkerValue->currentText());//маркер
    accept();
}

void SlotEditorWnd::on_btnCancel_clicked()
{
    reject();
}
