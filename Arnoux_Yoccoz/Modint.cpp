#include "Modint.h"
#include <cassert>

int Modint::integerMod(int a, int b){
    int result = a % b;
    return result >= 0 ? result : result + b;
}



Modint::Modint(int value, int modulus) :
    m_modulus(modulus)
{
    assert(modulus > 0);
    m_value = integerMod(value, modulus);
}
