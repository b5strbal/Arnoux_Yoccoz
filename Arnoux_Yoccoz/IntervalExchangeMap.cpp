/**
 * @file    IntervalExchangeMap.cpp
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the implementation for the IntervalExchangeMap class.
 */


#include "IntervalExchangeMap.h"


template <typename Type>
std::ostream& operator<<(std::ostream& Out, const std::vector<Type>& vec){
    for (int i = 0; i < vec.size(); i++) {
        Out << vec[i] << " ";
    }
    return Out;
}



Permutation::Permutation(const std::vector<int> functionValues)
    : m_functionValues(functionValues)
{
    assert(size() > 0);
    std::vector<char> isInTheRange(size(), 0);
    for (int i = 0; i < size(); i++) {
        assert(m_functionValues[i] < size());
        assert(isInTheRange[m_functionValues[i]] == 0);
        isInTheRange[m_functionValues[i]] = 1;
    }
}



Permutation Permutation::inverse() const{
    std::vector<int> newFunctionValues(size());
    for (int i = 0; i < size(); i++) {
        newFunctionValues[m_functionValues[i]] = i;
    }
    return Permutation(newFunctionValues);
}


std::ostream& operator<<(std::ostream& Out, const Permutation& permutation){
    for(long i = 0; i < permutation.size(); i++)
        std::cout << permutation.m_functionValues[i] << " ";
    return Out;
}
















IntervalExchangeMap::IntervalExchangeMap(const std::vector<floating_point_type> lengths,
                                         const Permutation permutation) 
{
    init(lengths, permutation);
}





IntervalExchangeMap::IntervalExchangeMap(const std::vector<floating_point_type> lengths,
                                         const Permutation permutation,
                                         floating_point_type twist) 
{
    init(lengths, permutation);
    if(twist <= 0 || twist >= 1)
        throw std::runtime_error("The twist must be strictly between 0 and 1");
    UnitIntervalPoint newDivPoint = applyInverseTo(UnitIntervalPoint(1 - twist));
    int intervalOfNewDivPoint = containingInterval(m_divPoints, newDivPoint);
    int intervalOfOneMinusTwist = containingInterval(m_divPointsAfterExchange, UnitIntervalPoint(1 - twist));
    
    if (m_divPoints[intervalOfNewDivPoint] == newDivPoint) {
        throw std::runtime_error("A twisted interval exchange must not be a regular interval exchange.");
    }
    
    // Finding the new lengths
    std::vector<floating_point_type> newLengths(m_lengths);
    floating_point_type smallerLength = newDivPoint.getPosition() - m_divPoints[intervalOfNewDivPoint].getPosition();
    newLengths.insert(newLengths.begin() + intervalOfNewDivPoint + 1, newLengths[intervalOfNewDivPoint] - smallerLength);
    newLengths[intervalOfNewDivPoint] = smallerLength;
    
    // Finding the new permutation
    std::vector<int> permutationInput(size() + 1);
    for (int i = 0; i < intervalOfNewDivPoint; i++) {
        if (m_permutation[i] < intervalOfOneMinusTwist) {
            permutationInput[i] = m_permutation[i] + size() - intervalOfOneMinusTwist;
        } else
            permutationInput[i] -= m_permutation[i] - intervalOfOneMinusTwist;
    }
    permutationInput[intervalOfNewDivPoint] = size();
    permutationInput[intervalOfNewDivPoint + 1] = 0;
    for (int i = intervalOfNewDivPoint + 2; i <= size(); i++) {
        if (m_permutation[i - 1] < intervalOfOneMinusTwist) {
            permutationInput[i] = m_permutation[i - 1] + size() - intervalOfOneMinusTwist;
        } else
            permutationInput[i] -= m_permutation[i - 1] - intervalOfOneMinusTwist;
    }
    init(newLengths, Permutation(permutationInput));
}





void IntervalExchangeMap::init(const std::vector<floating_point_type> lengths, const Permutation permutation){
    if(lengths.size() != permutation.size()){
        throw std::runtime_error("The number of intervals is ambiguous.");
    }
    int size = static_cast<int>(lengths.size());
    if (size == 0) throw std::runtime_error("The number of intervals must be at least 1.");
    for (int i = 0; i < size; i++) {
        if (lengths[i] <= 0)
            throw std::runtime_error("The length parameters an interval exchange map must be positive.");
    }

    floating_point_type total = 0;
    for (int i = 0; i < size; i++) {
        total += lengths[i];
    }
    
    m_lengths.resize(size);
    for (int i = 0; i < size; i++) {
        m_lengths[i] = lengths[i]/total;
    }
    
    m_divPoints.resize(size);
    m_divPoints[0] = 0;
    for (int i = 1; i < size; i++) {
        m_divPoints[i] = m_divPoints[i - 1] + m_lengths[i - 1];
    }
    
    m_permutation = permutation;
    m_inversePermutation = permutation.inverse();
    
    m_divPointsAfterExchange.resize(size);
    for (int i = 1; i < size; i++) {
        m_divPointsAfterExchange[i] = m_divPointsAfterExchange[i - 1] + m_lengths[m_inversePermutation[i - 1]];
    }
    
    m_translations.resize(size);
    for (int i = 0; i < size; i++) {
        m_translations[i] = m_divPointsAfterExchange[m_permutation[i]].getPosition() - m_divPoints[i].getPosition();
    }
}





std::ostream& operator<<(std::ostream& Out, const IntervalExchangeMap intervalExchange){
    Out << "Permutation: " << intervalExchange.m_permutation << "\n";
    Out << "Lenghts: " << intervalExchange.m_lengths;
    return Out;
}










UnitIntervalPoint IntervalExchangeMap::applyTo(const UnitIntervalPoint& point){
    return point + m_translations[containingInterval(m_divPoints, point)];
}


UnitIntervalPoint IntervalExchangeMap::applyInverseTo(const UnitIntervalPoint& point){
    return point - m_translations[m_inversePermutation[containingInterval(m_divPointsAfterExchange, point)]];
}







template <typename Type>
int IntervalExchangeMap::findInterval(const std::vector<Type>& separatingPoints,
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






