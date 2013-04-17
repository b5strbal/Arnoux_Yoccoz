#ifndef MODINT_H
#define MODINT_H

class Modint {

public:
    Modint(int value, int modulus);

    Modint& operator++() { m_value++; m_value %= m_modulus; return *this; }
    Modint& operator--() { m_value = (m_value == 0 ? m_modulus - 1 : m_value - 1); return *this; }
    Modint& operator+=(int a) { m_value = integerMod(m_value + a, m_modulus); return *this; }
    Modint& operator-=(int a) { m_value = integerMod(m_value - a, m_modulus); return *this; }
    friend Modint operator+(const Modint& modint, int a) { return Modint(modint.m_value + a, modint.m_modulus); }
    friend Modint operator-(const Modint& modint, int a) { return Modint(modint.m_value - a, modint.m_modulus); }

    operator int() const { return m_value; }
private:
    int m_value;
    int m_modulus;

    static int integerMod(int a, int b);
};

#endif // MODINT_H
