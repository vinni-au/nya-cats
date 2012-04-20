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
    model->setSlotFasetValue(m_slotIndex,"value",ui->cmbSlotValue->currentText());//имя
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
    ui->cmbSlotValue->setEnabled(true);
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
        ui->cmbSlotValue->setEnabled(false);
        ui->cmbSlotDomain->setModel(new QStringListModel());//опустошаем комбик доменов
        ui->cmbSlotValue->setModel(new QStringListModel());


        ui->cmbDefaultValue->addItem("0");

        ui->cmbMarkerType->clear();
        ui->cmbMarkerType->addItem("production");
        ui->cmbMarkerType->addItem("procedure");
    }
    else if(slotType == "frame")
    {
        ui->cmbSlotDomain->setEnabled(false);
        ui->cmbSlotValue->setEnabled(true);

        QMap<unsigned,QString> frames =  m_kbManager->frameNames();

        QMapIterator<unsigned,QString> i(frames);
        while (i.hasNext()) {
             i.next();
             ui->cmbDefaultValue->addItem(i.value());
         }
    }
}
