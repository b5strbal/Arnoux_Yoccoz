/**
 * @mainpage    Arnoux-Yoccoz
 *
 *              The main goal of the program is to construct Arnoux-Yoccoz-type pseudo-anosovs.
 */



#include <iostream>
#include <time.h>
#include "Foliation.h"
//#include "io.h"
#include "FoliationRP2.h"
#include "PerronFrobenius.h"

using namespace std;







int main (int argc, const char * argv[])
{
    Foliation f = arnouxYoccozFoliation(3);

    clock_t t1, t2;
    t1 = clock();
    
    f.generateSepSegments(10000);

    
    t2 = clock();
    float seconds = ((float)t2 - (float)t1)/CLOCKS_PER_SEC;
    cout << seconds;
    

  //  cout << arnouxYoccozRP2() << endl << endl;
  //  cout << FoliationSphere (arnouxYoccozRP2()) << endl << endl;
  //  cout << Foliation( arnouxYoccozRP2() ).rotateBy(1) << endl;
    
    /*
    try {
        for (int i = 0; i < 100; i++) {
            cout << FoliationDisk(WeighedTree(3)) << endl;
        }
    } catch (const std::exception& ex) {
        cout << "Error: " << ex.what();
    }
    */
    
    
    return 0;
}

