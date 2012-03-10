#include <QtGui/QApplication>
#include <QTextCodec>
#include "esmain.h"
#include "domainwnd.h"
#include "varswnd.h"
#include "ruleswnd.h"
#include "labeledtextbox.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    //VarsWnd w;
    ESmain w;
    //DomainWnd w;
    //RulesWnd  w;
    ///LabeledTextBox w;

    qDebug()<<"ололо";
    w.show();

    return a.exec();
}
