#ifndef KBEDITORWINDOW_HPP
#define KBEDITORWINDOW_HPP

#include <QMainWindow>

namespace Ui {
    class KBEditorWindow;
}

class KBEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit KBEditorWindow(QWidget *parent = 0);
    ~KBEditorWindow();

private:
    Ui::KBEditorWindow *ui;
};

#endif // KBEDITORWINDOW_HPP
