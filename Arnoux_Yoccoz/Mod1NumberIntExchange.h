#ifndef MOD1NUMBERINTEXCHANGE_H
#define MOD1NUMBERINTEXCHANGE_H

#include "Mod1Number.h"
#include "IntervalExchangeMap.h"

class IntervalExchangeBase;
class IntervalExchangeMap;

class Mod1NumberIntExchange : public Mod1Number {
    friend IntervalExchangeBase;
    friend IntervalExchangeMap;
public:
    Mod1NumberIntExchange(const IntervalExchangeBase* intExchange); // Constructs 0.
   // Mod1NumberIntExchange(const IntervalExchangeBase& intExchange, int intervalIndex);

    friend Mod1NumberIntExchange operator+(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);
    friend Mod1NumberIntExchange operator-(const Mod1NumberIntExchange&, const Mod1NumberIntExchange&);
    friend Mod1NumberIntExchange operator-(const Mod1NumberIntExchange&);

private:
    Mod1NumberIntExchange(const IntervalExchangeBase *intExchange,
                          const Mod1Number &mod1number,
                          const std::vector<int>& coefficients);
    Mod1NumberIntExchange(const IntervalExchangeBase *intExchange,
                          const Mod1Number &intervalLength,
                          int intervalIndex); // Constructor for lengths of intervals in int. echanges.

    const IntervalExchangeBase* m_intExchange;
    std::vector<int> m_coefficients;
};



#endif // MOD1NUMBERINTEXCHANGE_H
