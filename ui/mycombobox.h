#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QComboBox>
#include <QKeyEvent>

class MyComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MyComboBox(QWidget *parent = 0);
    void selectPrevious();
private:
    void keyPressEvent(QKeyEvent *e);

signals:
    void sigNewValueEntered(QString value);
public slots:
    void onCurrentIndexChanged(int index);

private:
    int currentInx;
    int previousInx;

};

#endif // MYCOMBOBOX_H
