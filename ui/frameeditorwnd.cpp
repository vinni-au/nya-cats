#include "frameeditorwnd.h"
#include "ui_frameeditorwnd.h"

FrameEditorWnd::FrameEditorWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameEditorWnd)
{
    ui->setupUi(this);
}

FrameEditorWnd::~FrameEditorWnd()
{
    delete ui;
}
