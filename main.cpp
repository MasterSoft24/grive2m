#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{

    QApplication::setStyle("fusion");
    QApplication a(argc, argv);
    MainWindow w;




    return a.exec();
}
