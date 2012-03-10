#ifndef RULESWND_H
#define RULESWND_H

#include <QMainWindow>
#include <QSplitter>
#include <QGroupBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include "labeledtextbox.h"

class RulesWnd : public QMainWindow
{
    Q_OBJECT
public:
    explicit RulesWnd(QWidget *parent = 0);

signals:

public slots:
    void retranslate();

private:
    QSplitter   *splMain;

    //верх
    QGroupBox   *grbListRules;
     QWidget     *wgtListRules;//нафег?
      QHBoxLayout *layListRules;
       QListWidget *lbListRules;
       QVBoxLayout *layBtnsListRules;
        QPushButton *btnAddRule;
        QPushButton *btnDeleteRule;

        QPushButton *btnUpRule;
        QPushButton *btnDownRule;

    //низ
   QWidget     *wgtRuleDetails;
    QVBoxLayout *layRuleDetails;
     QHBoxLayout *layNameAndReason;
      LabeledTextBox     *ltbNameRule;
      LabeledTextBox     *ltbReason;
     QSplitter    *splIfThen;
      QGroupBox    *grbIf;
       QWidget      *wgtIf;//нафег
        QHBoxLayout  *layIf;
         QListWidget  *lbIf;
         QVBoxLayout  *layBtnsIf;
          QPushButton  *btnAddIf;
          QPushButton  *btnEditIf;
          QPushButton  *btnDeleteIf;
          QPushButton  *btnUpIf;
          QPushButton  *btnDownIf;
      QGroupBox    *grbThen;
       QWidget      *wgtThen;//нафег
        QHBoxLayout  *layThen;
         QListWidget  *lbThen;
         QVBoxLayout  *layBtnsThen;
          QPushButton  *btnAddThen;
          QPushButton  *btnEditThen;
          QPushButton  *btnDeleteThen;
          QPushButton  *btnUpThen;
          QPushButton  *btnDownThen;


};

#endif // RULESWND_H
