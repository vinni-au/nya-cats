#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "ui/esmain.h"
//#include "ui/domainwnd.h"
//#include "ui/varswnd.h"
//#include "ui/ruleswnd.h"
//#include "ui/labeledtextbox.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    QTranslator t;
    if(t.load(":translations/qt_ru.qm"))
    {
        qDebug()<<"Переводы загружены";
        a.installTranslator(&t);
    }
    else
    {
        qDebug()<<"Переводы не загружены";
    }
    //transl1.load(":/translations/qt_ru.ts");
    //QCoreApplication::installTranslator(&transl1);
    //a.installTranslator(&transl);
    //VarsWnd w;
    ESmain w;

    if(argc>1)
        w.open(argv[1]);
    //DomainWnd w;
    //RulesWnd  w;
    ///LabeledTextBox w;

    w.show();

    return a.exec();
}
