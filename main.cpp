#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    //qputenv("QT_DEBUG_PLUGINS", 1);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
