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

class VarsWnd : public QMainWindow
{
    Q_OBJECT
public:
    explicit VarsWnd(QWidget *parent = 0);
    void retranslate();

signals:

public slots:

private:
    QSplitter   *splMain;

    //левая половина
    QWidget     *wgtLeft;
    QVBoxLayout *layLeft;
    QGroupBox   *grbVars;
    QVBoxLayout     *layVars;
    QListWidget *lswVars;

    //правая половина
    QVBoxLayout *layRidgt;
    QWidget     *wgtRight;

    QGroupBox   *grbActBtns;

    QVBoxLayout *layBtns;
    QPushButton *btnAddVar;
    QPushButton *btnEditVar;
    QPushButton *btnDeleteVar;

    QGroupBox   *grbType;
    QVBoxLayout *layType;
    QCheckBox   *chbDerivable;  //выводимая
    QCheckBox   *chbRequested;  //запрашиваемая

    QGroupBox   *grbDomain;
    QVBoxLayout *layDomain;
    QComboBox   *cmbDomain;

    QGroupBox   *grbQuestion;
    QVBoxLayout *layQuestion;
    QTextEdit  *tbQuestion;

};

#endif // VARSWND_H
