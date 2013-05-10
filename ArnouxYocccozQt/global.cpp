//
//  global.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 3/16/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "global.h"


std::size_t balazs::integerMod(int a, std::size_t b){
    int result = a % b;
    return result >= 0 ? result : result + b;
}

int balazs::gcd(int a, int b){
    return b == 0 ? a : gcd(b, a % b);
}











balazs::floating_point_type balazs::fracPart(floating_point_type x){ return x - floor(x); }
