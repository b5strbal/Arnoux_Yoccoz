#include "Mod1NumberIntExchange.h"
#include "TwistedIntervalExchangeMap.h"
#include <cassert>
#include <cmath>



balazs::Mod1NumberIntExchange::Mod1NumberIntExchange(const TwistedIntervalExchangeMap* twistedIntervalExchange,
                                             const Mod1Number &mod1number,
                                             const std::vector<int> &coefficients,
                                             int twistCoeff) :
    Mod1Number(mod1number),
    m_coefficients(coefficients),
    m_twistCoeff(twistCoeff),
    m_parent(twistedIntervalExchange)
{
    assert(twistedIntervalExchange != nullptr);
}


//balazs::Mod1NumberIntExchange::Mod1NumberIntExchange(const LengthsAndTwist* lengthsAndTwist) :
//    Mod1Number(0),
//    m_coefficients(lengthsAndTwist->lengths().size(), 0),
//    m_twistCoeff(0),
//    m_parent(lengthsAndTwist)
//{
//    assert(lengthsAndTwist != nullptr);
//}


//balazs::Mod1NumberIntExchange balazs::Mod1NumberIntExchange::constructZero(const LengthsAndTwist *lengthsAndTwist)
//{
//    return Mod1NumberIntExchange(lengthsAndTwist);
//}

//balazs::Mod1NumberIntExchange balazs::Mod1NumberIntExchange::constructTwist(const LengthsAndTwist *lengthsAndTwist)
//{
//    Mod1NumberIntExchange result(lengthsAndTwist);
//    result.Mod1Number::operator +=(lengthsAndTwist->twist());
//    result.m_twistCoeff = 1;
//    return result;
//}

//balazs::Mod1NumberIntExchange balazs::Mod1NumberIntExchange::constructLength(const LengthsAndTwist *lengthsAndTwist,
//                                                             std::size_t lengthIndex)
//{
//    Mod1NumberIntExchange result(lengthsAndTwist);
//    result.Mod1Number::operator +=(lengthsAndTwist->lengths()[lengthIndex]);
//    result.m_coefficients[lengthIndex] = 1;
//    return result;
//}










balazs::Mod1NumberIntExchange& balazs::Mod1NumberIntExchange::operator+=(const Mod1NumberIntExchange &rhs)
{
    assert(parent() != nullptr);
    assert(parent() == rhs.parent());

//    if(static_cast<long double>(*this) + static_cast<long double>(rhs) >= 1){
//        for(auto coeff : m_coefficients)
//            coeff--;
//    }

    Mod1Number::operator+=(rhs);
    for(std::size_t i = 0; i < m_coefficients.size(); i++){
        m_coefficients[i] += rhs.m_coefficients[i];
    }
    m_twistCoeff += rhs.twistCoeff();

    return *this;
}


balazs::Mod1NumberIntExchange balazs::Mod1NumberIntExchange::operator-() const
{
    assert(parent() != nullptr);
    std::vector<int> newCoefficients(m_coefficients.size());
    for(std::size_t i = 0; i < m_coefficients.size(); i++){
        newCoefficients[i] = -m_coefficients[i];
    }
//    if(static_cast<long double>(*this) > 0){
//        for(std::size_t i = 0; i < m_coefficients.size(); i++){
//            newCoefficients[i]++;
//        }
//    }
    return Mod1NumberIntExchange(m_parent, this->Mod1Number::operator-(), newCoefficients, -m_twistCoeff);
}



balazs::Mod1NumberIntExchange balazs::Mod1NumberIntExchange::shiftedTo(HDirection side) const
{
    return Mod1NumberIntExchange(m_parent, Mod1Number::shiftedTo(side), m_coefficients, m_twistCoeff);
}

void balazs::Mod1NumberIntExchange::adjustCoefficients()
{
    long double calculatedValue = 0;
    for(std::size_t i = 0; i < m_coefficients.size(); i++){
        calculatedValue += m_coefficients[i] * m_parent->lengths()[i];
    }
    calculatedValue += m_twistCoeff * m_parent->twist();

    int difference = round(static_cast<long double>(*this) - calculatedValue);

    for(std::size_t i = 0; i < m_coefficients.size(); i++){
        m_coefficients[i] += difference;
    }
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






