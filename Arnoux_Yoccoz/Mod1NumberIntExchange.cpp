#include "Mod1NumberIntExchange.h"

Mod1NumberIntExchange::Mod1NumberIntExchange(const IntervalExchangeBase *intExchange) :
    Mod1Number(),
    m_intExchange(intExchange),
    m_coefficients(intExchange.size(), 0)
{
}

Mod1NumberIntExchange::Mod1NumberIntExchange(const IntervalExchangeBase *intExchange, int intervalIndex) :
    Mod1Number(intExchange.lengths()[intervalIndex]),
    m_intExchange(intExchange),
    m_coefficients(intExchange.size(), 0)
{
    m_coefficients[intervalIndex] = 1;
}

Mod1NumberIntExchange Mod1NumberIntExchange::operator +(const Mod1NumberIntExchange & x,
                                                        const Mod1NumberIntExchange & y)
{
    assert(x.m_intExchange == y.m_intExchange);
    Mod1Number sum = static_cast<Mod1Number>(x) + static_cast<Mod1Number>(y);
    std::vector<int> newCoefficients(x.m_coefficients.size());
    for(int i = 0; i < x.m_coefficients.size(); i++){
        newCoefficients[i] = x.m_coefficients[i] + y.m_coefficients[i];
    }
    if(static_cast<floating_point_type>(x) + static_cast<floating_point_type>(y) >= 1){
        for(auto coeff : newCoefficients)
            coeff--;
    }
    return Mod1NumberIntExchange(x.m_intExchange, sum, newCoefficients);
}

Mod1NumberIntExchange Mod1NumberIntExchange::operator -(const Mod1NumberIntExchange & x,
                                                        const Mod1NumberIntExchange & y)
{
    return x + (-y);
}


Mod1NumberIntExchange::Mod1NumberIntExchange(const IntervalExchangeBase *intExchange,
                                             const Mod1Number &mod1number,
                                             const std::vector<int> &coefficients) :
    Mod1Number(mod1number),
    m_intExchange(intExchange),
    m_coefficients(coefficients)
{
}

Mod1NumberIntExchange::Mod1NumberIntExchange(const IntervalExchangeBase *intExchange,
                                             const Mod1Number &intervalLength,
                                             int intervalIndex) :
    Mod1Number(intervalLength),
    m_intExchange(intExchange),
    m_coefficients(intExchange.size(), 0)
{
    m_coefficients[intervalIndex] = 1;
}


Mod1NumberIntExchange Mod1NumberIntExchange::operator -(const Mod1NumberIntExchange & x)
{
    Mod1Number negativeOfx = -static_cast<Mod1Number>(x);
    std::vector<int> newCoefficients(x.m_coefficients.size());
    for(int i = 0; i < x.m_coefficients.size(); i++){
        newCoefficients[i] = -x.m_coefficients[i];
    }
    if(x.m_position > 0){
        for(int i = 0; i < x.m_coefficients.size(); i++){
            newCoefficients[i]++;
        }
    }
    return Mod1NumberIntExchange(x.m_intExchange, negativeOfx, newCoefficients);
}
