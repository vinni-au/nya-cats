#include "sloteditorwnd.h"
#include "ui_sloteditorwnd.h"

SlotEditorWnd::SlotEditorWnd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SlotEditorWnd)
{
    ui->setupUi(this);
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
