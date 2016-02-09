#include "mainwindow.h"
#include <QApplication>

#include "filelogger.h"

int main(int argc, char *argv[])
{
    FileLogger logger("nemo.log");
    qInstallMessageHandler(FileLogger::message);

    QApplication a(argc, argv);
    MainWindow w;    

    qApp->setQuitOnLastWindowClosed(false);

    return a.exec();
}
