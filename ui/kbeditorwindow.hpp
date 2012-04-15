#ifndef KBEDITORWINDOW_HPP
#define KBEDITORWINDOW_HPP

#include <QMainWindow>
#include "core/nkbmanager.h"
#include "sloteditorwnd.h"

namespace Ui {
    class KBEditorWindow;
}

class KBEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit KBEditorWindow(NKBManager *kbManager,QWidget *parent = 0);
    ~KBEditorWindow();

private slots:

    void on_btnAddFrame_clicked();

    void on_btnDeleteFrame_clicked();

    void on_btnAddSlot_clicked();

    void on_btnEditSlot_clicked();

    void on_btnDeleteSlot_clicked();

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::KBEditorWindow *ui;

    NKBManager *m_kbManager;
};

#endif // KBEDITORWINDOW_HPP
