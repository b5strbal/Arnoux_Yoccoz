/**
 * @file    IntervalExchangeMap.cpp
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the implementation for the IntervalExchangeMap class.
 */


#include "IntervalExchangeMap.h"






Permutation::Permutation(const std::vector<int> functionValues)
    : m_functionValues(functionValues)
{
    if (size() == 0) throw std::runtime_error("Empty permutation.");
    std::vector<char> isInTheRange(size(), 0);
    for (int i = 0; i < size(); i++) {
        if(m_functionValues[i] >= size() || m_functionValues[i] < 0)
            throw std::runtime_error("A permutation must be specified by integers between 0 and n-1.");
        if (isInTheRange[m_functionValues[i]] == 1)
            throw std::runtime_error("Permutations are one-to-one maps.");
        isInTheRange[m_functionValues[i]] = 1;
    }
}






Permutation operator*(const Permutation& p1, const Permutation& p2){
    if (p1.size() != p2.size()) {
        throw std::runtime_error("Two permutations must act on the same set in order to be composable.");
    }
    std::vector<int> compositionValues(p1.size());
    for (int i = 0 ; i < p1.size(); i++) {
        compositionValues[i] = p1[p2[i]];
    }
    return Permutation(compositionValues);
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


Permutation rotatingPermutation(int size, int rotationAmount){
    std::vector<int> functionValues(size);
    int normalizedAmount = integerMod(rotationAmount, size);
    for (int i = 0; i < size - normalizedAmount; i++) {
        functionValues[i] = i + normalizedAmount;
    }
    for (int i = size - normalizedAmount; i < size; i++) {
        functionValues[i] = i + normalizedAmount - size;
    }
    return Permutation(functionValues);
}









/*
FoliationDiskPairing::FoliationDiskPairing(const std::vector<int> functionValues) : Permutation(functionValues){
    for (int i = 0; i < size(); i++) {
        if (m_functionValues[i] == i) {
            throw std::runtime_error("In a pairing the pair of an element has to be different from itself.");
        }
        if (m_functionValues[m_functionValues[i]] != i) {
            throw std::runtime_error("The pair of a pair must be itself.");
        }
        if ((m_functionValues[i] - i) % 2 == 0) {
            throw std::runtime_error("There are crossing pairs. (The parity of indices of pairs must be different.)");
        }
    }
    
    // Finally checking that there are no crossings:
    
    assertRangeIsClosedUnderPair(m_functionValues, 1, m_functionValues[0] - 1);
    assertRangeIsClosedUnderPair(m_functionValues, m_functionValues[0] + 1, size() - 1);
}



void FoliationDiskPairing::assertRangeIsClosedUnderPair(const std::vector<int>& functionValues, int begin, int end){
    if (begin < end) {
        if (m_functionValues[begin] < begin || m_functionValues[begin] > end) {
            throw std::runtime_error("There are crossing pairs.");
        }
        if (end - begin > 2) {
            assertRangeIsClosedUnderPair(m_functionValues, begin + 1, m_functionValues[begin] - 1);
            assertRangeIsClosedUnderPair(m_functionValues, m_functionValues[begin] + 1, end);
        }
    }
}

*/














//----------------------//
// IntervalExchangeBase //
//----------------------//


IntervalExchangeBase::IntervalExchangeBase(const WeighedTree& wt){
    std::vector<floating_point_type> lengths;
    std::vector<int> pairing;
    wt.generateLengthsAndPairing(lengths, pairing);
    init(lengths, pairing);
}








IntervalExchangeBase::IntervalExchangeBase(const std::vector<floating_point_type>& lengths,
                                         const Permutation& permutation)
{
    init(lengths, permutation);
}





void IntervalExchangeBase::init(const std::vector<floating_point_type>& lengths, const Permutation& permutation){
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


















//---------------------//
// IntervalExchangeMap //
//---------------------//



IntervalExchangeMap::IntervalExchangeMap(const std::vector<floating_point_type>& lengths, const Permutation& permutation) :
    IntervalExchangeBase(lengths, permutation)
{
}







std::ostream& operator<<(std::ostream& Out, const IntervalExchangeMap intervalExchange){
    Out << "Permutation: " << intervalExchange.m_permutation << "\n";
    Out << "Lengths: " << intervalExchange.m_lengths;
    return Out;
}







UnitIntervalPoint IntervalExchangeMap::applyTo(const UnitIntervalPoint& point){
    return point + m_translations[containingInterval(m_divPoints, point)];
}


UnitIntervalPoint IntervalExchangeMap::applyInverseTo(const UnitIntervalPoint& point){
    return point - m_translations[m_inversePermutation[containingInterval(m_divPointsAfterExchange, point)]];
}
















//----------------------------//
// TwistedIntervalExchangeMap //
//----------------------------//


TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const std::vector<floating_point_type>& lengths,
                                                       const Permutation& permutation,
                                                       floating_point_type twist) :
    IntervalExchangeMap(lengths, permutation),
    m_originalLengths(m_lengths),
    m_originalPermutation(permutation),
    m_twist(twist)
{
    init(lengths, permutation, twist);
}



void TwistedIntervalExchangeMap::init(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist){
    if(twist <= 0 || twist >= 1)
        throw std::runtime_error("The twist must be strictly between 0 and 1");
    UnitIntervalPoint newDivPoint = applyInverseTo(UnitIntervalPoint(1 - twist));
    int intervalOfNewDivPoint = containingInterval(m_divPoints, newDivPoint);
    int intervalOfOneMinusTwist = containingInterval(m_divPointsAfterExchange, UnitIntervalPoint(1 - twist));
    m_indexOfFakeDivPoint = intervalOfNewDivPoint + 1;
    
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
    IntervalExchangeBase::init(newLengths, Permutation(permutationInput));
}




void TwistedIntervalExchangeMap::rotateBy(int rotationAmount){
    int normalizedAmount = integerMod(rotationAmount, originalSize());
    Permutation rotation = rotatingPermutation(originalSize(), normalizedAmount);
    m_originalPermutation = rotation * m_originalPermutation * rotation.inverse();
  //  m_originalLengths
}


















//-------------------------------//
// IntervalExchangeFoliationDisk //
//-------------------------------//


IntervalExchangeFoliationDisk::IntervalExchangeFoliationDisk(const WeighedTree& wt) :
    IntervalExchangeBase(wt)
{
}


UnitIntervalPoint IntervalExchangeFoliationDisk::applyTo(const UnitIntervalPoint& point){
    int interval = containingInterval(m_divPoints, point);
    return m_divPoints[m_permutation[interval]].getPosition() + m_lengths[interval] - (m_divPoints[interval].getPosition() - point.getPosition());
}



bool IntervalExchangeFoliationDisk::isTooCloseToDivPoint(const UnitIntervalPoint& point){
    int interval = containingInterval(m_divPoints, point);
    if (fabs(m_divPoints[interval + 1].getPosition() - point.getPosition()) < PRECISION ||
        fabs(point.getPosition() - m_divPoints[interval].getPosition()) < PRECISION)
    {
        return true;
    }
    return false;
}


std::ostream& operator<<(std::ostream& Out, const IntervalExchangeFoliationDisk intervalExchange){
    Out << "Lengths: " << intervalExchange.m_lengths << "\n";
    Out << "Pairing: " << intervalExchange.m_permutation;
    return Out;
}




