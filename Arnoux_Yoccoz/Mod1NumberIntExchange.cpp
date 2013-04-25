#include "Mod1NumberIntExchange.h"

Mod1NumberIntExchange::Mod1NumberIntExchange(int numIntervals) :
    Mod1Number(),
    m_coefficients(numIntervals, 0),
    m_twistCoeff(0)
{
}



Mod1NumberIntExchange::Mod1NumberIntExchange(const Mod1Number &mod1number,
                                             const std::vector<int> &coefficients,
                                             int twistCoeff) :
    Mod1Number(mod1number),
    m_coefficients(coefficients),
    m_twistCoeff(twistCoeff)
{
}

Mod1NumberIntExchange::Mod1NumberIntExchange(int numIntervals, const Mod1Number &intervalLength,
                                             int intervalIndex) :
    Mod1Number(intervalLength),
    m_coefficients(numIntervals, 0),
    m_twistCoeff(0)
{
    m_coefficients[intervalIndex] = 1;
}

Mod1NumberIntExchange::Mod1NumberIntExchange(int numIntervals, floating_point_type twist) :
    Mod1Number(twist),
    m_coefficients(numIntervals, 0),
    m_twistCoeff(1)
{
}







Mod1NumberIntExchange operator +(const Mod1NumberIntExchange & x, const Mod1NumberIntExchange & y)
{

    Mod1Number sum = static_cast<Mod1Number>(x) + static_cast<Mod1Number>(y);
    std::vector<int> newCoefficients(x.m_coefficients.size());
    for(unsigned int i = 0; i < x.m_coefficients.size(); i++){
        newCoefficients[i] = x.m_coefficients[i] + y.m_coefficients[i];
    }

    floating_point_type x_float = static_cast<floating_point_type>(x);
    floating_point_type y_float = static_cast<floating_point_type>(y);

    if(x_float + y_float >= 1){
        for(auto coeff : newCoefficients)
            coeff--;
    }
    return Mod1NumberIntExchange(sum, newCoefficients, x.m_twistCoeff + y.m_twistCoeff);
}

Mod1NumberIntExchange operator -(const Mod1NumberIntExchange & x, const Mod1NumberIntExchange & y)
{
    return x + (-y);
}




Mod1NumberIntExchange operator -(const Mod1NumberIntExchange & x)
{
    Mod1Number negativeOfx = -static_cast<Mod1Number>(x);
    std::vector<int> newCoefficients(x.m_coefficients.size());
    for(unsigned int i = 0; i < x.m_coefficients.size(); i++){
        newCoefficients[i] = -x.m_coefficients[i];
    }
    if(x.m_position > 0){
        for(unsigned int i = 0; i < x.m_coefficients.size(); i++){
            newCoefficients[i]++;
        }
    }
    return Mod1NumberIntExchange(negativeOfx, newCoefficients, -x.m_twistCoeff);
}
