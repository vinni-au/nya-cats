#ifndef LABELEDTEXTBOX_H
#define LABELEDTEXTBOX_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class LabeledTextBox : public QWidget
{
    Q_OBJECT
public:
    explicit LabeledTextBox(QWidget *parent = 0);

    void setLabel(QString label);
    void setText(QString text);
    QString getText();

signals:
    void textChanged(const QString & text);
    void textEdited(const QString & text);

public slots:


public:
    QVBoxLayout *layMain;
     QHBoxLayout *layLabel;
      QLabel      *lbLabel;
     QLineEdit   *tbTextBox;
};

#endif // LABELEDTEXTBOX_H
