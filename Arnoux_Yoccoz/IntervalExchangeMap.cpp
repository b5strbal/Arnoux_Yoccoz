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
            throw std::runtime_error("A permutation should be a one-to-one map.");
        isInTheRange[m_functionValues[i]] = 1;
    }
}


template <typename Type>
std::vector<Type> Permutation::applyAndCreateCopy(const std::vector<Type>& vec){
    if (size() != vec.size()) {
        throw std::runtime_error("A permutation can't act on a vector if the sizes are different.");
    }
    std::vector<Type> newVector(size());
    for (int i = 0; i < size(); i++) {
        newVector[m_functionValues[i]] = vec[i];
    }
    return newVector;
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


Permutation reversingPermutation(int size){
    std::vector<int> functionValues(size);
    for (int i = 0 ; i < size; i++) {
        functionValues[i] = size - 1 - i;
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
    m_divPoints[0] = UnitIntervalPoint(0);
    for (int i = 1; i < size; i++) {
        m_divPoints[i] = m_divPoints[i - 1] + m_lengths[i - 1];
        if (!(m_divPoints[i - 1] < m_divPoints[i])) {
            throw std::runtime_error("At least one interval in the interval exchange is too short.");
        }
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
    IntervalExchangeMap(lengths, permutation)
{
    init(lengths, permutation, twist);
}



void TwistedIntervalExchangeMap::init(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist){
    m_originalLengths = m_lengths;
    m_originalPermutation = permutation;
    m_twist = twist;
    
    
    int originalSize = static_cast<int>(lengths.size());
    UnitIntervalPoint oneMinusTwist(FracPart(1 - twist));
    UnitIntervalPoint newDivPoint = applyInverseTo(oneMinusTwist);
    int intervalOfNewDivPoint = containingInterval(m_divPoints, newDivPoint);
    if (intervalOfNewDivPoint == CONTAINING_INTERVAL_NOT_UNIQUE) {
        throw std::runtime_error("The specified data results in an immediate saddle connection of the foliation constructed by the\
                                 twisted interval exchange map.");
    }
    int intervalOfOneMinusTwist = containingInterval(m_divPointsAfterExchange, oneMinusTwist);
    m_indexOfFakeDivPoint = intervalOfNewDivPoint + 1;
    
    
    // Finding the new lengths
    std::vector<floating_point_type> newLengths(m_lengths);
    floating_point_type smallerLength = newDivPoint.getPosition() - m_divPoints[intervalOfNewDivPoint].getPosition();
    newLengths.insert(newLengths.begin() + intervalOfNewDivPoint + 1, newLengths[intervalOfNewDivPoint] - smallerLength);
    newLengths[intervalOfNewDivPoint] = smallerLength;
    
    // Finding the new permutation
    std::vector<int> permutationInput(originalSize + 1);
    for (int i = 0; i < intervalOfNewDivPoint; i++) {
        if (m_permutation[i] < intervalOfOneMinusTwist) {
            permutationInput[i] = m_permutation[i] + originalSize - intervalOfOneMinusTwist;
        } else
            permutationInput[i] = m_permutation[i] - intervalOfOneMinusTwist;
    }
    permutationInput[intervalOfNewDivPoint] = originalSize;
    permutationInput[intervalOfNewDivPoint + 1] = 0;
    for (int i = intervalOfNewDivPoint + 2; i <= originalSize; i++) {
        if (m_permutation[i - 1] < intervalOfOneMinusTwist) {
            permutationInput[i] = m_permutation[i - 1] + originalSize - intervalOfOneMinusTwist;
        } else
            permutationInput[i] = m_permutation[i - 1] - intervalOfOneMinusTwist;
    }
    IntervalExchangeBase::init(newLengths, Permutation(permutationInput));
    
    
    m_realSeparatingPoints.clear();
    int firstindex = 0;
    int secondindex = 1;
    while (firstindex + secondindex < 2 * originalSize + 2) {
        if (secondindex == originalSize + 1 || (firstindex < originalSize + 1 &&
                                                       m_divPoints[firstindex] < m_divPointsAfterExchange[secondindex])) {
            if (firstindex == m_indexOfFakeDivPoint) {  
                firstindex++;
            } else {
                m_realSeparatingPoints.push_back(m_divPoints[firstindex]);
                firstindex++;
            }
        } else {
            m_realSeparatingPoints.push_back(m_divPointsAfterExchange[secondindex]);
            secondindex++;
        }
    }
    for (int i = 1; i < 2 * originalSize; i++) {
        if (!(m_realSeparatingPoints[i - 1] < m_realSeparatingPoints[i])) {
            throw std::runtime_error("The specified data results in an immediate saddle connection of the foliation constructed by the\
                                     twisted interval exchange map.");
        }
    }
}




void TwistedIntervalExchangeMap::rotateBy(int rotationAmount){
    int normalizedAmount = integerMod(rotationAmount, originalSize());
    if (normalizedAmount == 0) {
        return;
    }
    std::vector<floating_point_type> newLengths(originalSize());
    std::rotate_copy(m_originalLengths.begin(), m_originalLengths.end() - normalizedAmount, m_originalLengths.end(), newLengths.begin());

    // Postcomposing the original permutation by a rotation results in the same twisted interval exchange map if we modify the twist accordingly.
    // When the length vector is rotated, we must therefore precompose the original permutation by a rotation. By the previous remark,
    // postcomposing is not important as long as we choose the rigth twist.

    Permutation newPermutation = m_originalPermutation * rotatingPermutation(originalSize(), - normalizedAmount);

    int indexOfDivPointGoingToZero = m_indexOfFakeDivPoint + normalizedAmount < size() ? size() - normalizedAmount : size() - normalizedAmount - 1;
    floating_point_type rotationDistance = 1 - m_divPoints[indexOfDivPointGoingToZero].getPosition();
    floating_point_type newTwist = m_twist + rotationDistance;
    
    IntervalExchangeBase::init(newLengths, newPermutation);
    init(newLengths, newPermutation, newTwist);
}


void TwistedIntervalExchangeMap::reflect(){
    std::vector<floating_point_type> newLengths(originalSize());
    std::reverse_copy(m_originalLengths.begin(), m_originalLengths.end(), newLengths.begin());
    
    // Now, in contract to rotateBy, we have to pre- and post-compose our premutation
    
    Permutation reverse = reversingPermutation(originalSize());
    Permutation newPermutation = reverse * m_originalPermutation * reverse;

    // It is not hard to see that after reflecting the whole picture, the new twist is simply the negative of the old one.
    
    floating_point_type newTwist = -m_twist;
    IntervalExchangeBase::init(newLengths, newPermutation);
    init(newLengths, newPermutation, newTwist);
}



void TwistedIntervalExchangeMap::invert(){
    std::vector<floating_point_type> newLengths(m_originalPermutation.applyAndCreateCopy(m_originalLengths));
    Permutation newPermutation = m_originalPermutation.inverse();
    floating_point_type newTwist = -m_twist;
    
    IntervalExchangeBase::init(newLengths, newPermutation);
    init(newLengths, newPermutation, newTwist);
}






std::ostream& operator<<(std::ostream& Out, const TwistedIntervalExchangeMap twistedIntervalExchange){
    Out << "Lengths: " << twistedIntervalExchange.m_lengths << "\n";
    Out << "Original lengths: " << twistedIntervalExchange.m_originalLengths << "\n";
    Out << "Translations: " << twistedIntervalExchange.m_translations << "\n";
    
    return Out;
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
    return UnitIntervalPoint(m_divPoints[m_permutation[interval]].getPosition() + m_lengths[interval] - (m_divPoints[interval].getPosition() - point.getPosition()));
}





std::ostream& operator<<(std::ostream& Out, const IntervalExchangeFoliationDisk intervalExchange){
    Out << "Lengths: " << intervalExchange.m_lengths << "\n";
    Out << "Pairing: " << intervalExchange.m_permutation;
    return Out;
}




