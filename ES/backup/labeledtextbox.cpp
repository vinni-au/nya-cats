#include "labeledtextbox.h"

LabeledTextBox::LabeledTextBox(QWidget *parent) :
    QWidget(parent)
{
    layMain=new QVBoxLayout();
    layLabel=new QHBoxLayout();
    lbLabel=new QLabel();
    lbLabel->setText("olololo");
    tbTextBox=new QLineEdit();

    layLabel->addWidget(lbLabel);
    //layLabel->addSpacing(20);

    layMain->addLayout(layLabel);
    layMain->addWidget(tbTextBox);


    //layMain->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Minimum));

    this->setLayout(layMain);

    this->setMaximumHeight(60);
    //lbLabel->setGeometry(0,0,20,10);
    //tbTextBox->setGeometry(0,20,20,10);

}

void
LabeledTextBox::setLabel(QString label)
{
    this->lbLabel->setText(label);
}
void
LabeledTextBox::setText(QString text)
{
    this->tbTextBox->setText(text);
}
QString
LabeledTextBox::getText()
{
    return this->tbTextBox->text();
}
