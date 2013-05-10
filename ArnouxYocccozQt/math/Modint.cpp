#include "Modint.h"
#include "../global.h"
#include <cassert>



balazs::Modint& balazs::Modint::operator+=(int a) {
    m_value = integerMod(m_value + a, m_modulus);
    return *this;
}


balazs::Modint::Modint(unsigned int value, unsigned int modulus) :
    m_modulus(modulus)
{
    assert(modulus > 0);
    m_value = integerMod(value, modulus);
}



balazs::Modint& balazs::operator-=(Modint& m, int a) {
    m += -a; return m;
}

balazs::Modint& balazs::operator++(Modint& m) {
    m += 1; return m;
}

balazs::Modint& balazs::operator--(Modint& m) {
    m -= 1; return m;
}

balazs::Modint balazs::operator+(const Modint& modint, int a) {
    Modint sum = modint; sum += a;
    return sum;
}

balazs::Modint balazs::operator-(const Modint& modint, int a) {
    return modint + (-a);
}
