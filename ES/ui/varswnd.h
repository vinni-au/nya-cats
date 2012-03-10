#ifndef VARSWND_H
#define VARSWND_H

#include <QMainWindow>
#include <QSplitter>
#include <QGroupBox>
#include <QListWidget>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>
#include "mylistview.h"
#include "core/variable.h"
#include "core/datamodels.h"
#include "saver.h"
#include "mycombobox.h"

class VarsWnd : public QMainWindow
{
    Q_OBJECT
public:
    explicit VarsWnd(QWidget *parent = 0);
    void retranslate();
    bool isEditMode();
signals:
    void sigClosed();
public slots:
    void onAddClick();
    void onEditClick();
    void onDeleteClick();

    void onDerivableChanged(bool checked);
    void onAskingChanged(bool checked);

    void onDomainChanged(QString str);
    void onDomainActivated(QString str);

    void onQuestionChanged();



    void onVarChanged(QModelIndex index);

    void onDataLoaded();

    void onEditModeClick();
    void onCloseClick();

    void onOkSaveClick();
    void onCancelClick();
    void maybeClose();//на самом деле не закрывает форму

    void onContextDomainAdded(QString newDomain);

    void onChuprinaEditModeEnterClick();
private:
    void editStart();//для работы с режимом редактирования
    void editStop();
    void closeEvent(QCloseEvent *event);
    void chuprinaEditMode(bool enter);
private:
    bool editMode;

    QWidget *wgtMain;
    QLayout *layVertical;

    QSplitter   *splMain;

    //левая половина
    QWidget     *wgtLeft;
    QVBoxLayout *layLeft;
    QGroupBox   *grbVars;
    QVBoxLayout     *layVars;
    MyListView *lswVars;

    //правая половина
    QVBoxLayout *layRidgt;
    QWidget     *wgtRight;

    QGroupBox   *grbActBtns;

    QVBoxLayout *layBtns;
    QPushButton *btnAddVar;
    QPushButton *btnEditVar;//имя
    QPushButton *btnChuprinaEditMode;//вход в режим редактирования
    QPushButton *btnDeleteVar;

    QGroupBox   *grbType;
    QVBoxLayout *layType;
    QCheckBox   *chbDerivable;  //выводимая
    QCheckBox   *chbRequested;  //запрашиваемая

    QGroupBox   *grbDomain;
    QVBoxLayout *layDomain;
    MyComboBox   *cmbDomain;

    QGroupBox   *grbQuestion;
    QVBoxLayout *layQuestion;
    QTextEdit  *tbQuestion;


    QHBoxLayout *layBtnsEditMode;// лэйаут для режима редактирования
     QWidget     *wgtBtnsEditMode;
      QPushButton *btnEditMode;
      QPushButton *btnClose;
      QPushButton *btnOkSave;
      QPushButton *btnCancel;
};

#endif // VARSWND_H
