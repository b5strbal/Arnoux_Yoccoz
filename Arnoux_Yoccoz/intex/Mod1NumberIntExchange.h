#ifndef MOD1NUMBERINTEXCHANGE_H
#define MOD1NUMBERINTEXCHANGE_H

#include "LengthsAndTwist.h"

namespace balazs{


class Mod1NumberIntExchange : public Mod1Number {
public:
    Mod1NumberIntExchange() : m_lengthsAndTwist(nullptr) {} // Constructs uninitialized obj.

    static Mod1NumberIntExchange constructZero(const LengthsAndTwist* lengthsAndTwist);
    static Mod1NumberIntExchange constructTwist(const LengthsAndTwist* lengthsAndTwist);
    static Mod1NumberIntExchange constructLength(const LengthsAndTwist* lengthsAndTwist,
                                                 unsigned int lengthIndex);

    Mod1NumberIntExchange& operator+=(const Mod1NumberIntExchange& rhs);
    Mod1NumberIntExchange operator-() const;

    const std::vector<int>& coefficients() const { return m_coefficients; }
    int twistCoeff() const { return m_twistCoeff; }
private:
    Mod1NumberIntExchange(const LengthsAndTwist* lengthsAndTwist,
                          const Mod1Number &mod1number,
                          const std::vector<int>& coefficients,
                          int twistCoeff); // Constructor with all member variables as arguments.
    Mod1NumberIntExchange(const LengthsAndTwist* lengthsAndTwist); // Constructs 0.

    std::vector<int> m_coefficients;
    int m_twistCoeff;
    const LengthsAndTwist* m_lengthsAndTwist;
};


Mod1NumberIntExchange operator+(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);
Mod1NumberIntExchange operator-(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);

}


#endif // MOD1NUMBERINTEXCHANGE_H
