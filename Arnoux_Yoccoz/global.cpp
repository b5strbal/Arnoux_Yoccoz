//
//  global.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 3/16/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "global.h"


int integerMod(int a, int b){
    assert(b > 0);
    int result = a % b;
    return result >= 0 ? result : result + b;
}



int modAdd(int x, int y, int mod){
    assert(mod > 0);
    return integerMod(x + y, mod);
}



floating_point_type FracPart(floating_point_type x){ return x - floor(x); }
