#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.hpp"

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
    MainWindow w;
    w.show();

    return a.exec();
}
