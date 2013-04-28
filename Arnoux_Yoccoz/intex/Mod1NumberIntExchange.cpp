#include "Mod1NumberIntExchange.h"





balazs::Mod1NumberIntExchange::Mod1NumberIntExchange(const LengthsAndTwist* lengthsAndTwist,
                                             const Mod1Number &mod1number,
                                             const std::vector<int> &coefficients,
                                             int twistCoeff) :
    Mod1Number(mod1number),
    m_coefficients(coefficients),
    m_twistCoeff(twistCoeff),
    m_lengthsAndTwist(lengthsAndTwist)
{
    assert(lengthsAndTwist != nullptr);
}


balazs::Mod1NumberIntExchange::Mod1NumberIntExchange(const LengthsAndTwist* lengthsAndTwist) :
    Mod1Number(0),
    m_coefficients(lengthsAndTwist->lengths().size(), 0),
    m_twistCoeff(0),
    m_lengthsAndTwist(lengthsAndTwist)
{
    assert(lengthsAndTwist != nullptr);
}


balazs::Mod1NumberIntExchange balazs::Mod1NumberIntExchange::constructZero(const LengthsAndTwist *lengthsAndTwist)
{
    return Mod1NumberIntExchange(lengthsAndTwist);
}

balazs::Mod1NumberIntExchange balazs::Mod1NumberIntExchange::constructTwist(const LengthsAndTwist *lengthsAndTwist)
{
    Mod1NumberIntExchange result(lengthsAndTwist);
    result.Mod1Number::operator +=(lengthsAndTwist->twist());
    result.m_twistCoeff = 1;
    return result;
}

balazs::Mod1NumberIntExchange balazs::Mod1NumberIntExchange::constructLength(const LengthsAndTwist *lengthsAndTwist,
                                                             unsigned int lengthIndex)
{
    Mod1NumberIntExchange result(lengthsAndTwist);
    result.Mod1Number::operator +=(lengthsAndTwist->lengths()[lengthIndex]);
    result.m_coefficients[lengthIndex] = 1;
    return result;
}










balazs::Mod1NumberIntExchange& balazs::Mod1NumberIntExchange::operator+=(const Mod1NumberIntExchange &rhs)
{
    assert(m_lengthsAndTwist != nullptr);
    assert(m_lengthsAndTwist == rhs.m_lengthsAndTwist);
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


balazs::Mod1NumberIntExchange balazs::Mod1NumberIntExchange::operator-() const
{
    assert(m_lengthsAndTwist != nullptr);
    std::vector<int> newCoefficients(m_coefficients.size());
    for(unsigned int i = 0; i < m_coefficients.size(); i++){
        newCoefficients[i] = -m_coefficients[i];
    }
    if(static_cast<floating_point_type>(*this) > 0){
        for(unsigned int i = 0; i < m_coefficients.size(); i++){
            newCoefficients[i]++;
        }
    }
    return Mod1NumberIntExchange(m_lengthsAndTwist, this->Mod1Number::operator-(), newCoefficients, -m_twistCoeff);
}





balazs::Mod1NumberIntExchange balazs::operator +(const Mod1NumberIntExchange & x, const Mod1NumberIntExchange & y)
{
    Mod1NumberIntExchange sum = x;
    sum += y;
    return sum;
}


balazs::Mod1NumberIntExchange balazs::operator -(const Mod1NumberIntExchange & x, const Mod1NumberIntExchange & y)
{
    return x + (-y);
}






