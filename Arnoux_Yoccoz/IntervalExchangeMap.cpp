/**
 * @file    IntervalExchangeMap.cpp
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the implementation for the IntervalExchangeMap class.
 */


#include "IntervalExchangeMap.h"


Permutation::Permutation(const std::vector<unsigned long> functionValues)
    : m_functionValues(functionValues)
{
    assert(size() > 0);
    std::vector<char> isInTheRange(size(), 0);
    for (long i = 0; i < size(); i++) {
        assert(m_functionValues[i] < size());
        assert(isInTheRange[m_functionValues[i]] == 0);
        isInTheRange[m_functionValues[i]] = 1;
    }
}



Permutation Permutation::inverse() const{
    std::vector<unsigned long> newFunctionValues(size());
    for (long i = 0; i < size(); i++) {
        newFunctionValues[m_functionValues[i]] = i;
    }
    return Permutation(newFunctionValues);
}


std::ostream& operator<<(std::ostream& Out, const Permutation& permutation){
    for(long i = 0; i < permutation.size(); i++)
        std::cout << permutation.m_functionValues[i] << " ";
    return Out;
}

















TwistedIntervalExchange::TwistedIntervalExchange(const std::vector<floating_point_type> lengths,
                                                 const Permutation permutation,
                                                 floating_point_type twist) :
    m_lengths(lengths),
    m_permutation(permutation),
    m_twist(twist),
    m_inversePermutation(permutation.inverse()),
    m_divPoints(size()),
    m_divPointsAfterExchange(size()),
    m_translations(size())
{
    assert(lengths.size() == permutation.size());
    assert(size() > 0);
    for (long i = 0; i < size(); i++) {
        assert(lengths[i] > 0);
    }
    for (long i = 1; i < size(); i++) {
        m_divPoints[i] = m_divPoints[i - 1] + m_lengths[i - 1];
        m_divPointsAfterExchange[i] = m_divPointsAfterExchange[i - 1] + m_lengths[m_inversePermutation[i - 1]];
    }
    for (long i = 0; i < size(); i++) {
        m_translations[i] = m_divPointsAfterExchange[m_permutation[i]].getPosition() - m_divPoints[i].getPosition();
    }
}





UnitIntervalPoint TwistedIntervalExchange::applyTo(const UnitIntervalPoint& point){
    return point + m_translations[containingInterval(m_divPoints, point)];
}


UnitIntervalPoint TwistedIntervalExchange::applyInverseTo(const UnitIntervalPoint& point){
    return point - m_translations[m_inversePermutation[containingInterval(m_divPointsAfterExchange, point)]];
}







template <typename Type>
int TwistedIntervalExchange::findInterval(const std::vector<Type>& separatingPoints,
                                      const Type& point,
                                      int start,
                                      int end)
{
    if (start == end){
        return start;
    }
    int mid = (start + end + 1) / 2;
    if (point < separatingPoints[mid]) {
        return findInterval(separatingPoints, point, start, mid - 1);
    } else
        return findInterval(separatingPoints, point, mid, end);
}






