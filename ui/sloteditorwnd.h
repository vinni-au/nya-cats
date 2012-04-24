#ifndef SLOTEDITORWND_H
#define SLOTEDITORWND_H

#include <QDialog>
#include <QModelIndex>
#include <QStringListModel>
#include <QInputDialog>
#include "core/frame_model/nframenetmodel.h"
#include "core/nkbmanager.h"
#include "ruleswnd.h"

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


private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_cmbSlotType_currentIndexChanged(int index);

    void on_btnEditMarker_clicked();

    void onProductionAdded(NProduction *production,bool newProd);
    void on_cmbMarkerType_currentIndexChanged(const QString &arg1);

private:
    Ui::SlotEditorWnd *ui;
    QModelIndex m_slotIndex;
    NKBManager *m_kbManager;
};

#endif // SLOTEDITORWND_H
