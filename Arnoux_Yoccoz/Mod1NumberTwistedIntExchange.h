#ifndef MOD1NUMBERTWISTEDINTEXCHANGE_H
#define MOD1NUMBERTWISTEDINTEXCHANGE_H

#include "Mod1NumberIntExchange.h"

class Mod1NumberTwistedIntExchange
{
public:
    Mod1NumberTwistedIntExchange(const TwistedIntervalExchangeMap* twistedIntExchange);

    Mod1NumberTwistedIntExchange operator+(const Mod1NumberTwistedIntExchange& x,
                                           const Mod1NumberTwistedIntExchange& y);
    Mod1NumberTwistedIntExchange operator-(const Mod1NumberTwistedIntExchange& x,
                                           const Mod1NumberTwistedIntExchange& y);
    Mod1NumberTwistedIntExchange operator-(const Mod1NumberTwistedIntExchange& x);

private:
    const TwistedIntervalExchangeMap* m_twistedIntExchange;
    Mod1NumberIntExchange m_mod1NumberIntExchange;
    int m_twistCoeff;
};

#endif // MOD1NUMBERTWISTEDINTEXCHANGE_H
