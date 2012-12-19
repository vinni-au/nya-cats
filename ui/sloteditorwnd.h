#ifndef SLOTEDITORWND_H
#define SLOTEDITORWND_H

#include <QDialog>
#include <QModelIndex>
#include <QStringListModel>
#include <QInputDialog>
#include "core/frame_model/nframenetmodel.h"
#include "core/nkbmanager.h"
#include "ruleswnd.h"
#include "codeeditor.hpp"
#include "proceditor.h"


namespace Ui {
    class SlotEditorWnd;
}

class SlotEditorWnd : public QDialog
{
    Q_OBJECT

public:
    explicit SlotEditorWnd(QModelIndex slotIndex,NKBManager *kbManager,QWidget *parent = 0);
    ~SlotEditorWnd();

private:
    bool dataIsValid();
    bool slotExists();
signals:

public slots:


private slots:

    void on_cmbSlotType_currentIndexChanged(int index);

    void on_btnEditMarker_clicked();

    void onProductionAdded(NProduction *production,bool newProd);
    void onProcAdded(NProc *proc,bool newProc);
    void on_cmbMarkerType_currentIndexChanged(const QString &arg1);

    void on_cmbSlotDomain_currentIndexChanged(const QString &arg1);

    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::SlotEditorWnd *ui;
    QModelIndex m_slotIndex;
    NKBManager *m_kbManager;
};

#endif // SLOTEDITORWND_H
