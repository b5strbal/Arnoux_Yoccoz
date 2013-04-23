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
    std::ostringstream s;
    s << f;
    qDebug() << QString::fromStdString(s.str());
   // return a.exec();
}
