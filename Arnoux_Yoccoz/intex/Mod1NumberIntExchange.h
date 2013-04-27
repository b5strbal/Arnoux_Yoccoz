#ifndef MOD1NUMBERINTEXCHANGE_H
#define MOD1NUMBERINTEXCHANGE_H

#include "Mod1Number.h"

class IntervalExchangeBase;

class Mod1NumberIntExchange : public Mod1Number {
public:
    Mod1NumberIntExchange() : m_intExchange(nullptr) {} // default constructor
    Mod1NumberIntExchange(const IntervalExchangeBase* intExchange); // Constructs 0.
    Mod1NumberIntExchange(const IntervalExchangeBase* intExchange, floating_point_type twist); // Constructor for the twist.
    Mod1NumberIntExchange(const IntervalExchangeBase* intExchange,
                          const Mod1Number &intervalLength,
                          int intervalIndex); // Constructor for lengths of intervals in interval exchanges.

    Mod1NumberIntExchange& operator+=(const Mod1NumberIntExchange& rhs);
    Mod1NumberIntExchange operator-() const;



    const std::vector<int>& coefficients() const { return m_coefficients; }
    int twistCoeff() const { return m_twistCoeff; }

private:
    Mod1NumberIntExchange(const IntervalExchangeBase* intExchange,
                          const Mod1Number &mod1number,
                          const std::vector<int>& coefficients,
                          int twistCoeff); // Constructor with all member variables as arguments.


    std::vector<int> m_coefficients;
    int m_twistCoeff;
    const IntervalExchangeBase* m_intExchange;
};


Mod1NumberIntExchange operator+(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);
Mod1NumberIntExchange operator-(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);



#endif // MOD1NUMBERINTEXCHANGE_H
