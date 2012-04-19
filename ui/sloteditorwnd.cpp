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

}

void SlotEditorWnd::on_buttonBox_rejected()
{

}
