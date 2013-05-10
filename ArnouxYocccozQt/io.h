//
//  io.h
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 2/19/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#ifndef Arnoux_Yoccoz_io_h
#define Arnoux_Yoccoz_io_h

#include <iostream>
#include "FoliationRP2.h"

using namespace std;













void PerformOperation(FoliationRP2* Foliation){
    bool Quit = false;
    while(!Quit) {
        PrintTitle("PERFORM AN OPERATION");
        cout << "Choose one:" << endl;
        cout << "- List good shifted separatrix segments (s)" << endl;
        cout << "- List good curves (c)" << endl;
        cout << "- Search for pseudo-anosovs (p)" << endl;
        cout << "- More info (i)" << endl;
        cout << "- Choose another foliation (f)" << endl;
        cout << "- Quit (q)" << endl;
        cout << "(Press key and ENTER)" << endl;
        
        char c = 0;
        while (c != 's' && c != 'c' && c != 'p' && c != 'i' && c != 'f' && c != 'q') {
            c = GetFirstCharOfLine();
        }
        switch (c) {
            case 's':
            {
                PrintTitle("LISTING GOOD SHIFTED SEPARATRIX SEGMENTS");
                cout << "You can choose between concise and verbose listing.\n";
                cout << "Verbose lists a lot of information, and most of the time it is unnecessary.\n";
                cout << "Concise lists only the depths of good segments, and only odd ones (this is not a big loss of information since it is almost true that 2k+1 is a good depth for a certain separatrix and side if and only if 2k is good as well).\n";
                cout << "Choose between two types of listing (unless you really need all information, verbose is not recommended):\n";
                cout << "- Concise (c)\n";
                cout << "- Verbose (v)\n";
                cout << "(Press key and ENTER)\n";

                char c = 0;
                while (c != 'c' && c != 'v') {
                    c = GetFirstCharOfLine();
                }
                cout << "Enter the depth of search for good shifted separatrix segments. (A depth of 1 million typically takes a few seconds, and the running time is linear in depth.)\n\n";
                
                switch (c) {
                    case 'c':
                        Foliation->PrintGoodShiftedSeparatrixSegmentsConcise(GetDepth());
                        break;
                        
                    case 'v':
                        Foliation->PrintGoodShiftedSeparatrixSegmentsVerbose(GetDepth());
                        break;
                }
                
                WaitForEnter();
                break;
            }
            case 'c':
                PrintTitle("LISTING GOOD CURVES");
                cout << "Enter the depth of search for good curves.\n\n";
                Foliation->PrintGoodCurves(GetDepth());
                WaitForEnter();
                break;
            case 'p':
                PrintTitle("SEARCH FOR PSEUDO-ANOSOVS");
                cout << "Enter the depth of search for pseudo-anosovs.\n\n";
                Foliation->PrintPseudoAnosovs(GetDepth());
                WaitForEnter();
                break;
            case 'i':
                PrintOperationInfo();
                break;
            case 'f':
                Quit = true;
                break;
            case 'q':
                throw -1; // exiting
                break;
        }
    }
}




#endif
