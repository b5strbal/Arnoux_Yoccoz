#ifndef MOD1NUMBERINTEXCHANGE_H
#define MOD1NUMBERINTEXCHANGE_H

#include "Mod1Number.h"


class Mod1NumberIntExchange : public Mod1Number {
    friend class IntervalExchangeBase;
  //  friend IntervalExchangeMap;
public:

    friend Mod1NumberIntExchange operator+(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);
    friend Mod1NumberIntExchange operator-(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);
    friend Mod1NumberIntExchange operator-(const Mod1NumberIntExchange&);

private:
    Mod1NumberIntExchange(); // Constructs 0.
    Mod1NumberIntExchange(const Mod1Number &mod1number,
                          const std::vector<int>& coefficients,
                          int twistCoeff); // Constructor with all member variables as arguments.
    Mod1NumberIntExchange(const Mod1Number &intervalLength,
                          int intervalIndex); // Constructor for lengths of intervals in interval exchanges.
    Mod1NumberIntExchange(floating_point_type twist); // Constructor for the twist.

    std::vector<int> m_coefficients;
    int m_twistCoeff;
};



#endif // MOD1NUMBERINTEXCHANGE_H
