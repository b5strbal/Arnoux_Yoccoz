#include "windows/WindowManager.h"
#include <QApplication>
//#include <QDebug>
//#include <QString>
//#include <sstream>
#include "fol/Foliation.h"
#include "scc/SepSegmentDatabase.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    WindowManager w;

   // balazs::Foliation fol(3);
   // balazs::SepSegmentDatabase(fol).generateSepSegments(20000);

    return a.exec();
}
