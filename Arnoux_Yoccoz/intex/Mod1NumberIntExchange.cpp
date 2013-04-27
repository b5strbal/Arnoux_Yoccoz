#include "Mod1NumberIntExchange.h"
#include "IntervalExchangeBase.h"

Mod1NumberIntExchange::Mod1NumberIntExchange(const IntervalExchangeBase* intExchange) :
    Mod1Number(),
    m_coefficients(intExchange->size(), 0),
    m_twistCoeff(0),
    m_intExchange(intExchange)
{
}



Mod1NumberIntExchange::Mod1NumberIntExchange(const IntervalExchangeBase* intExchange,
                                             const Mod1Number &mod1number,
                                             const std::vector<int> &coefficients,
                                             int twistCoeff) :
    Mod1Number(mod1number),
    m_coefficients(coefficients),
    m_twistCoeff(twistCoeff),
    m_intExchange(intExchange)
{
}

Mod1NumberIntExchange::Mod1NumberIntExchange(const IntervalExchangeBase* intExchange, const Mod1Number &intervalLength,
                                             int intervalIndex) :
    Mod1Number(intervalLength),
    m_coefficients(intExchange->size(), 0),
    m_twistCoeff(0),
    m_intExchange(intExchange)
{
    m_coefficients[intervalIndex] = 1;
}

Mod1NumberIntExchange::Mod1NumberIntExchange(const IntervalExchangeBase *intExchange, floating_point_type twist) :
    Mod1Number(twist),
    m_coefficients(intExchange->size(), 0),
    m_twistCoeff(1),
    m_intExchange(intExchange)
{
}



Mod1NumberIntExchange& Mod1NumberIntExchange::operator+=(const Mod1NumberIntExchange &rhs)
{
    Mod1Number::operator+=(rhs);
    for(unsigned int i = 0; i < m_coefficients.size(); i++){
        m_coefficients[i] += rhs.m_coefficients[i];
    }

    if(static_cast<floating_point_type>(*this) + static_cast<floating_point_type>(rhs) >= 1){
        for(auto coeff : m_coefficients)
            coeff--;
    }

    return *this;
}


Mod1NumberIntExchange Mod1NumberIntExchange::operator-() const
{
    std::vector<int> newCoefficients(m_coefficients.size());
    for(unsigned int i = 0; i < m_coefficients.size(); i++){
        newCoefficients[i] = -m_coefficients[i];
    }
    if(m_position > 0){
        for(unsigned int i = 0; i < m_coefficients.size(); i++){
            newCoefficients[i]++;
        }
    }
    return Mod1NumberIntExchange(m_intExchange, Mod1Number::operator-(), newCoefficients, -m_twistCoeff);
}





Mod1NumberIntExchange operator +(const Mod1NumberIntExchange & x, const Mod1NumberIntExchange & y)
{
    Mod1NumberIntExchange sum = x;
    sum += y;
    return sum;
}


Mod1NumberIntExchange operator -(const Mod1NumberIntExchange & x, const Mod1NumberIntExchange & y)
{
    return x + (-y);
}





