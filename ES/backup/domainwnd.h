#ifndef DOMAINWND_H
#define DOMAINWND_H

#include <QWidget>
#include <QSplitter>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QLabel>

class DomainWnd : public QMainWindow
{
    Q_OBJECT
public:
    explicit DomainWnd(QWidget *parent = 0);
    void retranslate();
signals:

public slots:
    void onAddDomain();
    void onEditDomain();
    void onDeleteDomain();

    void onAddValue();
    void onEditValue();
    void onDeleteValue();

private:
    QSplitter   *splitter;//разделитель

    QWidget     *wDomainPanel;
    QVBoxLayout *hlayDomainName;
    QLabel      *lbDomainName;
    QListWidget *lwDomainName;
    QPushButton *btnAddDomain;
    QPushButton *btnEditDomain;
    QPushButton *btnDeleteDomain;

    QWidget     *wValuePanel;
    QVBoxLayout *hlayValues;
    QLabel      *lbValue;
    QListWidget *lwValues;
    QPushButton *btnAddValue;
    QPushButton *btnEditValue;
    QPushButton *btnDeleteValue;


};

#endif // DOMAINWND_H
