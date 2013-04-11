//
//  global.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 3/16/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "global.h"


int Modint::integerMod(int a, int b){
    int result = a % b;
    return result >= 0 ? result : result + b;
}

int gcd(int a, int b){
    return b == 0 ? a : gcd(b, a % b);
}







Modint::Modint(int value, int modulus) :
    m_modulus(modulus)
{
    assert(modulus > 0);
    m_value = integerMod(value, modulus);
}








floating_point_type FracPart(floating_point_type x){ return x - floor(x); }
