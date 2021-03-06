#ifndef EXPREDITOR_H
#define EXPREDITOR_H

#include <QDialog>
#include "core/varmodel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include "core/datamodels.h"
#include <QStringListModel>
#include "mycombobox.h"
#include "core/expr.h"
#include "saver.h"

class ExprEditor : public QDialog
{
    Q_OBJECT
public:
    enum TypeExpr {IfExpr,ThenExpr};
    enum TypeAction{AddAction,EditAction};
    explicit ExprEditor(TypeExpr typeExpr,TypeAction typeAction, QWidget *parent = 0);

    void retranslate();
    Expr* getExpr();

    void selectVar(QModelIndex index);
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
     QHBoxLayout    *layDown;
      QPushButton   *btnAddNewExpr;
      QPushButton   *btnOK;
      QPushButton   *btnCancel;
};

#endif // EXPREDITOR_H
