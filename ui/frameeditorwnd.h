#ifndef FRAMEEDITORWND_H
#define FRAMEEDITORWND_H

#include <QWidget>

namespace Ui {
    class FrameEditorWnd;
}

class FrameEditorWnd : public QWidget
{
    Q_OBJECT

public:
    explicit FrameEditorWnd(QWidget *parent = 0);
    ~FrameEditorWnd();

private:
    Ui::FrameEditorWnd *ui;
};

#endif // FRAMEEDITORWND_H
