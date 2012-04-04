#ifndef DOMAINWND_H
#define DOMAINWND_H

#include <QWidget>
#include <QSplitter>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QLabel>
#include <QAction>
#include <QShortcut>
#include <QMessageBox>
#include <QCloseEvent>
#include "core/domains/domainmodel.h"
#include "mylistview.h"
#include "saver.h"


class DomainWnd : public QMainWindow
{
    Q_OBJECT
public:
    explicit DomainWnd(DomainModel* model,QWidget *parent = 0);
    void retranslate();
    bool isEditMode();

    void setModel(DomainModel* model);
signals:
    void sigClosed();
public slots:
    void onAddDomain();
    void onEditDomain();
    void onDeleteDomain();

    void onAddValue();
    void onEditValue();
    void onDeleteValue();

    void onDomainSelected(QModelIndex index);

    void onShortcutAdd();
    void onShortcutDelete();
    void onShortcutSwitch();

    void onDataLoaded();
    void maybeClose();//на самом деле не закрывает форму
    void onCloseClick();

    void onOkSaveClick();
    void onCancelClick();
    void onEditModeClick();
private:
    void editStart();//для работы с режимом редактирования
    void editStop();
    void closeEvent(QCloseEvent *event);
private:
    DomainModel* m_domainModel;

    bool editMode;
    QWidget *wgtMain;
    QLayout *layVertical;

    QSplitter   *splitter;//разделитель

    QWidget     *wDomainPanel;
    QVBoxLayout *hlayDomainName;
    QLabel      *lbDomainName;

    //QListWidget *lwDomainName;
    MyListView   *lwDomainName;

    QPushButton *btnAddDomain;
    QPushButton *btnEditDomain;
    QPushButton *btnDeleteDomain;

    QWidget     *wValuePanel;
    QVBoxLayout *hlayValues;
    QLabel      *lbValue;
    MyListView   *lwValues;
    QPushButton *btnAddValue;
    QPushButton *btnEditValue;
    QPushButton *btnDeleteValue;

    QHBoxLayout *layBtnsEditMode;// лэйаут для режима редактирования
     QWidget     *wgtBtnsEditMode;
      QPushButton *btnEditMode;
      QPushButton *btnClose;
      QPushButton *btnOkSave;
      QPushButton *btnCancel;
};

#endif // DOMAINWND_H
