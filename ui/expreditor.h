#ifndef EXPREDITOR_H
#define EXPREDITOR_H

#include <QDialog>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include <QStringListModel>
#include "mycombobox.h"
#include "core/production/expr.h"
#include "saver.h"
#include "core/nkbmanager.h"
#include <QSpinBox>

class ExprEditor : public QDialog
{
    Q_OBJECT
public:
    enum TypeExpr {IfExpr,ThenExpr};
    enum TypeAction{AddAction,EditAction};
    explicit ExprEditor(NKBManager* manager,QModelIndex slotIndex,QString domain,TypeExpr typeExpr,TypeAction typeAction, QWidget *parent = 0);

    void retranslate();
    Expr* getExpr();

    void selectVar(QModelIndex index);
    void selectVar(QString var);
    void selectSign(QString sign);
    void selectVal(QString val);

signals:
    void varAdded(QString varName);
    void valAdded(QString varName,QString value);

public slots:
    void onCurrentVarChanged(int inx);
    void onOkClick();
    void onCancelClick();
    void onOkAndReturnClick();

    virtual void reject();

    void onVarContextAdded(QString newVar);
    void onValueContextAdded(QString newVal);
private:
    bool dataIsValid();
public:
    NKBManager *m_kbManager;
    QModelIndex m_slotIndex;
    QString m_domain;//домен слота, в котором продукция
    TypeExpr    typeExpr;
    TypeAction  typeAction;
    bool startAgain;

    //форма
    QVBoxLayout *layMain;
     QHBoxLayout    *layUp;
      QVBoxLayout    *layVar;
       QLabel          *lbVar;
       MyComboBox       *cmbVar;
      QVBoxLayout    *laySign;
       QLabel          *lbSign;
       QComboBox       *cmbSign;
      QVBoxLayout    *layVal;
       QLabel          *lbVal;
       MyComboBox       *cmbVal;

      QVBoxLayout    *layArSign;
       QLabel          *lbArSign;
       QComboBox     *cmbArSign;
      QVBoxLayout    *laySummer;
       QLabel          *lbSummer;
       QSpinBox      *spbSummer;

     QHBoxLayout    *layDown;
      QPushButton   *btnAddNewExpr;
      QPushButton   *btnOK;
      QPushButton   *btnCancel;
};

#endif // EXPREDITOR_H
