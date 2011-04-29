#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
