#include "esmain.h"


ESmain::ESmain(QWidget *parent) :
    QMainWindow(parent)
{
    mdiArea=new QMdiArea(this);
    this->setCentralWidget(mdiArea);
    mdiArea->setDocumentMode(true);

    DomainWnd *domainWnd=new DomainWnd();
    mdiArea->addSubWindow(domainWnd);

    VarsWnd *varsWnd=new VarsWnd();
    mdiArea->addSubWindow(varsWnd);

    RulesWnd *rulesWnd=new RulesWnd();
    mdiArea->addSubWindow(rulesWnd);

    mdiArea->cascadeSubWindows();
    mdiArea->setTabShape(QTabWidget::Rounded);
    mdiArea->setViewMode(QMdiArea::TabbedView);
}

ESmain::~ESmain()
{

}
