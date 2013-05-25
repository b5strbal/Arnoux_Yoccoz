#ifndef MOD1NUMBERINTEXCHANGE_H
#define MOD1NUMBERINTEXCHANGE_H

#include "Mod1Number.h"
#include <vector>

namespace balazs{

//class LengthsAndTwist;
class TwistedIntervalExchangeMap;

class Mod1NumberIntExchange : public Mod1Number {
    friend class TwistedIntervalExchangeMap;
public:
    Mod1NumberIntExchange() : m_parent(nullptr) {} // Constructs uninitialized obj.
    virtual ~Mod1NumberIntExchange() = default;
//    static Mod1NumberIntExchange constructZero(const LengthsAndTwist* lengthsAndTwist);
//    static Mod1NumberIntExchange constructTwist(const LengthsAndTwist* lengthsAndTwist);
//    static Mod1NumberIntExchange constructLength(const LengthsAndTwist* lengthsAndTwist,
//                                                 std::size_t lengthIndex);

    // operations are allowed only objects of the same signature
    const TwistedIntervalExchangeMap* parent() const { return m_parent; }

    Mod1NumberIntExchange& operator+=(const Mod1NumberIntExchange& rhs);
    Mod1NumberIntExchange operator-() const;

    Mod1NumberIntExchange shiftedTo(HDirection side) const;
    void adjustCoefficients();
    const std::vector<int>& coefficients() const { return m_coefficients; }
    int twistCoeff() const { return m_twistCoeff; }

private:
    Mod1NumberIntExchange(const TwistedIntervalExchangeMap* twistedIntervalExchange,
                          const Mod1Number &mod1number,
                          const std::vector<int>& coefficients,
                          int twistCoeff); // Constructor with all member variables as arguments.
   // Mod1NumberIntExchange(const TwistedIntervalExchange* twistedIntervalExchange); // Constructs 0.

    std::vector<int> m_coefficients;
    int m_twistCoeff;
    const TwistedIntervalExchangeMap* m_parent;
};


Mod1NumberIntExchange operator+(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);
Mod1NumberIntExchange operator-(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);

}


#endif // MOD1NUMBERINTEXCHANGE_H
