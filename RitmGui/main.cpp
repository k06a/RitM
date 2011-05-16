#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QDir>
#include "mainwindow.h"

#include <QLabel>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    
    QApplication app(argc, argv);
    
    QDir::setCurrent(QApplication::applicationDirPath());
    QTranslator translator;
    translator.load("qt_ru");
    app.installTranslator(&translator);

    MainWindow window;
    window.show();

    return app.exec();
}
