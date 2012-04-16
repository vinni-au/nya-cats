#ifndef SLOTEDITORWND_H
#define SLOTEDITORWND_H

#include <QDialog>
#include <QModelIndex>
#include "core/frame_model/nframenetmodel.h"
#include "core/nkbmanager.h"

namespace Ui {
    class SlotEditorWnd;
}

class SlotEditorWnd : public QDialog
{
    Q_OBJECT

public:
    explicit SlotEditorWnd(QModelIndex slotIndex,NKBManager *kbManager,QWidget *parent = 0);
    ~SlotEditorWnd();

public slots:
    void onAddSlot();
    void onDeleteSlot();

private:
    Ui::SlotEditorWnd *ui;
    QModelIndex m_slotIndex;
    NKBManager *m_kbManager;
};

#endif // SLOTEDITORWND_H
