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
}

SlotEditorWnd::~SlotEditorWnd()
{
    delete ui;
}

void SlotEditorWnd::onAddSlot()
{

}

void SlotEditorWnd::onDeleteSlot()
{

}
