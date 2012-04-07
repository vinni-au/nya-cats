#ifndef SLOTEDITORWND_H
#define SLOTEDITORWND_H

#include <QDialog>

namespace Ui {
    class SlotEditorWnd;
}

class SlotEditorWnd : public QDialog
{
    Q_OBJECT

public:
    explicit SlotEditorWnd(QWidget *parent = 0);
    ~SlotEditorWnd();

public slots:
    void onAddSlot();
    void onDeleteSlot();

private:
    Ui::SlotEditorWnd *ui;
};

#endif // SLOTEDITORWND_H
