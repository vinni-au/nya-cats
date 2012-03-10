#ifndef RESULTSWND_H
#define RESULTSWND_H

#include <QWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMdiArea>
#include <QSplitter>
#include <QGroupBox>
#include <QTreeWidget>
#include <QMdiSubWindow>
#include "core/datamodels.h"



class ResultsWnd : public QWidget
{
    Q_OBJECT
public:
    explicit ResultsWnd(QWidget *parent = 0);

signals:

public slots:
    void slotAddRule(QString ruleName,int result);
    void slotAddVar(QString varName,QString varValue);
private:
    QTreeWidgetItem* createRuleItem(QString ruleName,int result);

public:
    QSplitter   *splMain;
    QHBoxLayout *layMain;
        QGroupBox   *gbRules;
            QVBoxLayout *layGBRules;
                QMdiArea    *mdiArea;
                    QTreeWidget *treeAllRules;
                    QTreeWidget *treeAcceptedRules;
        QGroupBox   *gbVars;
            QVBoxLayout *layGBVars;
                QListWidget *lbVars;
};

#endif // RESULTSWND_H
