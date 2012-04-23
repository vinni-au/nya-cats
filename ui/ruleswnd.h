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
#include <QAction>
#include <QShortcut>
#include <QMessageBox>
#include "labeledtextbox.h"

#include "mylistview.h"

#include "core/production/rule.h"

#include "expreditor.h"
#include "core/production/rulemodel.h"
#include "core/production/nproduction.h"


class RulesWnd : public QMainWindow
{
    Q_OBJECT
public:
    explicit RulesWnd(NProduction *production, QWidget *parent = 0);
    virtual ~RulesWnd();
    bool isEditMode();
signals:
    void sigClosed();
    void sigProductionAdded(NProduction *production);
public slots:
    void retranslate();

    void onCurrentRuleChanged(QModelIndex index);
    void onRuleNameChanged(QString text);
    void onRuleReasonChanged(QString text);

    void onAddRuleClick();
    void onEditRuleClick();
    void onDeleteRuleClick();
    void onMoveUpRuleClick();

    void onEditModeClick();
    void onCloseClick();

    void onOkSaveClick();
    void onCancelClick();

    void onAddIfClick();
    void onEditIfClick();
    void onDelIfClick();

    void onAddThenClick();
    void onEditThenClick();
    void onDelThenClick();

    void onShotcutAdd();
    void onShotcutDelete();
    void onShortcutSwitch();

    void onDataLoaded();
    void maybeClose();//на самом деле не закрывает форму


private:
    void editStart();//для работы с режимом редактирования
    void editStop();
    void chuprinaEditMode(bool enter);

    bool event(QEvent *event);
    void closeEvent(QCloseEvent *event);


private:
    RuleModel* ruleModel;

    bool editMode;
    QWidget  *esMain;

    QSplitter   *splMain;

    //верх
    QGroupBox   *grbListRules;
     QWidget     *wgtListRules;//нафег?
      QHBoxLayout *layListRules;
       MyListView *lbListRules;
       QVBoxLayout *layBtnsListRules;
        QPushButton *btnAddRule;
        QPushButton *btnEditRule;
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
         MyListView  *lbIf;
         QVBoxLayout  *layBtnsIf;
          QPushButton  *btnAddIf;
          QPushButton  *btnEditIf;
          QPushButton  *btnDeleteIf;
          QPushButton  *btnUpIf;
          QPushButton  *btnDownIf;
      QGroupBox    *grbThen;
       QWidget      *wgtThen;//нафег
        QHBoxLayout  *layThen;
         MyListView  *lbThen;
         QVBoxLayout  *layBtnsThen;
          QPushButton  *btnAddThen;
          QPushButton  *btnEditThen;
          QPushButton  *btnDeleteThen;
          QPushButton  *btnUpThen;
          QPushButton  *btnDownThen;
     QHBoxLayout *layBtnsEditMode;// лэйаут для режима редактирования
      QWidget     *wgtBtnsEditMode;
       QPushButton *btnEditMode;
       QPushButton *btnClose;
       QPushButton *btnOkSave;
       QPushButton *btnCancel;

};

#endif // RULESWND_H
