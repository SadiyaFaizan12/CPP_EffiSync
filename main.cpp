#include "mainwindow.h"

#include <QApplication>
#include <QTimer>
#include <QTime>
#include <QStringList>
#include<QtMultimedia/QtMultimedia>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.move(10,20);
    return a.exec();
}
