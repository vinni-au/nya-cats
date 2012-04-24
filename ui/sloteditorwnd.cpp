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


    QString slotType = model->getSlotFasetValue(slotIndex,"slot_type").toString() ;
    int slotTypeIndex = ui->cmbSlotType->findText(slotType);
    ui->cmbSlotType->setCurrentIndex(slotTypeIndex);


    //домен
    ui->cmbSlotDomain->setModel( kbManager->getDomainModel()  );

    QString slotDomain = model->getSlotFasetValue(slotIndex,"domain").toString() ;
    int slotDomainIndex = ui->cmbSlotDomain->findText(slotDomain);
    ui->cmbSlotDomain->setCurrentIndex(slotDomainIndex);
    //значение


    //значение по умолчанию

    //if(ui->cmbSlotType->currentText())
    //наследование

    ui->cmbInheritance->addItem("same");
    ui->cmbInheritance->addItem("override");

    QString slotInheritance = model->getSlotFasetValue(slotIndex,"inheritance").toString() ;
    int slotInheritanceIndex = ui->cmbInheritance->findText(slotInheritance);
    ui->cmbInheritance->setCurrentIndex(slotInheritanceIndex);
    //тип маркера
    ui->cmbMarkerType->addItem("domain");
    ui->cmbMarkerType->addItem("production");
    ui->cmbMarkerType->addItem("procedure");

    QString slotMarkerType = model->getSlotFasetValue(slotIndex,"marker_type").toString() ;
    int slotMarkerTypeIndex = ui->cmbMarkerType->findText(slotMarkerType);
    ui->cmbMarkerType->setCurrentIndex(slotMarkerTypeIndex);
    //значение маркера
    QString slotMarkerValue = model->getSlotFasetValue(slotIndex,"marker").toString() ;
    ui->cmbMarkerValue->setCurrentIndex( ui->cmbMarkerValue->findText( slotMarkerValue ) );
}

SlotEditorWnd::~SlotEditorWnd()
{
    delete ui;
}



void SlotEditorWnd::on_buttonBox_accepted()
{
    //проверка??
    NFramenetModel* model = m_kbManager->getFrameNetModel();
    model->setSlotFasetValue(m_slotIndex,"name",ui->lineEdit->text().trimmed());//имя
    model->setSlotFasetValue(m_slotIndex,"slot_type",ui->cmbSlotType->currentText());//имя
    model->setSlotFasetValue(m_slotIndex,"domain",ui->cmbSlotDomain->currentText());//имя
    //model->setSlotFasetValue(m_slotIndex,"value",ui->cmbSlotValue->currentText());//имя
    model->setSlotFasetValue(m_slotIndex,"default_value",ui->cmbDefaultValue->currentText());//имя
    model->setSlotFasetValue(m_slotIndex,"inheritance",ui->cmbInheritance->currentText());//имя
    model->setSlotFasetValue(m_slotIndex,"marker_type",ui->cmbMarkerType->currentText());//имя
    model->setSlotFasetValue(m_slotIndex,"marker",ui->cmbMarkerValue->currentText());//имя
}

void SlotEditorWnd::on_buttonBox_rejected()
{

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
    //ui->cmbSlotValue->setEnabled(true);
    ui->cmbDefaultValue->setEnabled(true);
    ui->cmbInheritance->setEnabled(true);
    ui->cmbMarkerType->setEnabled(true);
    ui->cmbMarkerValue->setEnabled(true);


    if(slotType == "string")
    {

    }
    else if(slotType == "int")
    {
        ui->cmbSlotDomain->setEnabled(false);
        //ui->cmbSlotValue->setEnabled(false);
        ui->cmbSlotDomain->setModel(new QStringListModel());//опустошаем комбик доменов
        //ui->cmbSlotValue->setModel(new QStringListModel());


        ui->cmbDefaultValue->addItem("0");

        ui->cmbMarkerType->clear();
        ui->cmbMarkerType->addItem("production");
        ui->cmbMarkerType->addItem("procedure");
    }
    else if(slotType == "frame")
    {
        ui->cmbSlotDomain->setEnabled(false);
        //ui->cmbSlotValue->setEnabled(true);

        QMap<unsigned,QString> frames =  m_kbManager->frameNames();

        QMapIterator<unsigned,QString> i(frames);
        while (i.hasNext()) {
             i.next();
             ui->cmbDefaultValue->addItem(i.value());
         }
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


        RulesWnd *rWnd = new RulesWnd(m_kbManager,production,newProd,this);
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
    QMessageBox::information(this,"SlotEditorWnd::onProductionAdded","Опачки, продукция добавлена.",QMessageBox::NoButton);
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
    m_kbManager->mayBeSave();
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

    }
    else if(markerType == "procedure")
    {
        ui->cmbMarkerValue->clear();
        QStringList names = m_kbManager->getProceduresNames();
        ui->cmbMarkerValue->addItems(names);
    }
}
