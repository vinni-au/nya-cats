#include "resultswnd.h"

ResultsWnd::ResultsWnd(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle("Объяснение");

    layMain = new QHBoxLayout();       this->setLayout(layMain);
    splMain = new QSplitter();         layMain->addWidget(splMain);
    splMain->setChildrenCollapsible(false);
    {
        gbRules = new QGroupBox();      splMain->addWidget(gbRules);
            gbRules->setTitle("Правила");
        {
            layGBRules = new QVBoxLayout(); gbRules->setLayout(layGBRules);
            {
                mdiArea = new QMdiArea();      layGBRules->addWidget(mdiArea);
                {
                    treeAllRules = new QTreeWidget();  mdiArea->addSubWindow(treeAllRules)->setVisible(true);
                    treeAllRules->setWindowTitle("Все правила");
                    treeAllRules->setHeaderHidden(true);

                    //treeAllRules->setModel(DataModels::instance()->ruleModel);
                    treeAcceptedRules = new QTreeWidget();  mdiArea->addSubWindow(treeAcceptedRules)->setVisible(true);
                    treeAcceptedRules->setWindowTitle("Сработавшие правила");
                    treeAcceptedRules->setHeaderHidden(true);
                }
                mdiArea->setTabShape(QTabWidget::Rounded);
                mdiArea->setViewMode(QMdiArea::TabbedView);

            }
        }


        gbVars = new QGroupBox();      splMain->addWidget(gbVars);
        gbVars->setTitle("Означенные переменные");
        {
            layGBVars = new QVBoxLayout(); gbVars->setLayout(layGBVars);
            {
                lbVars  = new QListWidget();   layGBVars->addWidget(lbVars);
            }
        }
    }


}

QTreeWidgetItem*
ResultsWnd::createRuleItem(QString ruleName,int result)
{
    QModelIndex ruleIndex = DataModels::instance()->ruleModel->indexByName(ruleName);
    QModelIndex ifsIndex =DataModels::instance()->ruleModel->index(1,0,ruleIndex);
    QModelIndex thensIndex =DataModels::instance()->ruleModel->index(2,0,ruleIndex);
    QModelIndex reasonIndex = DataModels::instance()->ruleModel->index(0,0,ruleIndex);


    QTreeWidgetItem *ruleItem = new QTreeWidgetItem();
    ruleItem->setText(0,ruleName);

    if(result == 0)//Accepted
    {
        ruleItem->setIcon(0,QIcon(":/rulePics/Accepted.png"));
    }
    else
    {
        ruleItem->setIcon(0,QIcon(":/rulePics/Rejected.png"));
    }

   // treeAllRules->addTopLevelItem(ruleItem);



    QTreeWidgetItem *ifs = new QTreeWidgetItem();
    ifs->setText(0,"if");
    ruleItem->addChild(ifs);

    QModelIndex ifIndex = DataModels::instance()->ruleModel->index(0,0,ifsIndex);
    while(ifIndex.isValid())
    {
        QTreeWidgetItem *ifIt = new QTreeWidgetItem();
        ifIt->setText(0,DataModels::instance()->ruleModel->data(ifIndex,Qt::DisplayRole).toString()   );
        ifs->addChild( ifIt);
        ifIndex=DataModels::instance()->ruleModel->index(ifIndex.row()+1,0,ifsIndex);
    }

    QTreeWidgetItem *thens = new QTreeWidgetItem();
    thens->setText(0,"then");
    ruleItem->addChild(thens);

    QModelIndex thenIndex = DataModels::instance()->ruleModel->index(0,0,thensIndex);
    while(thenIndex.isValid())
    {
        QTreeWidgetItem *thenIt = new QTreeWidgetItem();
        thenIt->setText(0,DataModels::instance()->ruleModel->data(thenIndex,Qt::DisplayRole).toString()   );
        thens->addChild( thenIt);
        thenIndex=DataModels::instance()->ruleModel->index(thenIndex.row()+1,0,thensIndex);
    }

    QTreeWidgetItem *reasonItem = new QTreeWidgetItem();
    reasonItem->setText(0,"Reason: "+ DataModels::instance()->ruleModel->data(reasonIndex ,Qt::DisplayRole).toString());
    ruleItem->addChild(reasonItem);

    return ruleItem;
}

void
ResultsWnd::slotAddRule(QString ruleName, int result)
{
    if(result == 0)//accepted
    {
        QTreeWidgetItem *ruleItem1 = createRuleItem(ruleName,result);
        treeAcceptedRules->addTopLevelItem(ruleItem1);
    }


    QTreeWidgetItem *ruleItem = createRuleItem(ruleName,result);
    treeAllRules->addTopLevelItem(ruleItem);


    treeAllRules->expandAll();
    treeAcceptedRules->expandAll();
}

void
ResultsWnd::slotAddVar(QString varName, QString varValue)
{

    lbVars->addItem(varName+" = "+"'"+varValue+"'");

}
