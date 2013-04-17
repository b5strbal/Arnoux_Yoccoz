#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QString>
#include "../Arnoux_Yoccoz/Foliation.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    Foliation f = arnouxYoccozFoliation(3);
    
    qDebug() << QString::fromStdString(f.print());
   // return a.exec();
}
