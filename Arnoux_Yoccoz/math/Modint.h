#ifndef MODINT_H
#define MODINT_H

namespace balazs{


int integerMod(int a, int b);



class Modint {
public:
    Modint(unsigned int value, unsigned int modulus);
    Modint& operator+=(int a);
    operator unsigned int() const { return m_value; }
private:
    unsigned int m_value;
    unsigned int m_modulus;
};



Modint& operator-=(Modint& m, int a);
Modint& operator++(Modint& m);
Modint& operator--(Modint& m);
Modint operator+(const Modint& modint, int a);
Modint operator-(const Modint& modint, int a);

}


#endif // MODINT_H
