#include "mainwindow.h"
#include "QStyle"
#include "QFile"
#include <QTextStream>
#include <QStyleFactory>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication::setStyle("cleanlooks");
    QApplication a(argc, argv);
     a.setStyle(QStyleFactory::create("fusion"));




    MainWindow w;
    w.show();

    return a.exec();
}
