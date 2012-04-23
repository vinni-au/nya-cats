#ifndef KBEDITORWINDOW_HPP
#define KBEDITORWINDOW_HPP

#include <QMainWindow>
#include "core/nkbmanager.h"
#include "sloteditorwnd.h"
#include "./../editor/arrow.hpp"
#include <QAction>
#include <QCloseEvent>

namespace Ui {
    class KBEditorWindow;
}

class KBEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit KBEditorWindow(NKBManager *kbManager,QWidget *parent = 0);
    ~KBEditorWindow();


    void setSimpleView(bool isSimple);

private slots:
    void on_btnAddFrame_clicked();
    void on_btnDeleteFrame_clicked();
    void on_btnAddSlot_clicked();
    void on_btnEditSlot_clicked();
    void on_btnDeleteSlot_clicked();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void onSimpleViewSwitched();
    void on_btn_addIsa_toggled(bool checked);
    void on_btn_addSub_toggled(bool checked);
    void arrowAdded(Arrow* arrow);
    void frameSelectedOnDiagram(unsigned id);
    void frameDeletedOnDiagram(unsigned id);
    void treeviewSelectionChanged(QItemSelection,QItemSelection);

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void frameSelectedOnTreeView(unsigned id);

private:
    Ui::KBEditorWindow *ui;

    NKBManager *m_kbManager;

    bool m_simpleView;
};

#endif // KBEDITORWINDOW_HPP
