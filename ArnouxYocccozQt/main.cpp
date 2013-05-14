#include "windows/WindowManager.h"
#include <QApplication>
#include <QDebug>
#include <QString>
#include <sstream>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    WindowManager w;


    return a.exec();
}
