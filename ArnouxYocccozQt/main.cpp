#include "windows/MainWindow.h"
#include <QApplication>
#include <QDebug>
#include <QString>
#include <sstream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    balazs::Foliation f = balazs::arnouxYoccozFoliation(3);
//    std::ostringstream s;
//    s << f;
//    qDebug() << QString::fromStdString(s.str());
    return a.exec();
}
