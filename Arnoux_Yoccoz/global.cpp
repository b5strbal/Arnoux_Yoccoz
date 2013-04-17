//
//  global.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 3/16/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "global.h"




int gcd(int a, int b){
    return b == 0 ? a : gcd(b, a % b);
}











floating_point_type FracPart(floating_point_type x){ return x - floor(x); }
