/**
 * @file    IntervalExchangeMap.cpp
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the implementation for the IntervalExchangeMap class.
 */


#include "IntervalExchangeMap.h"
#include "Modint.h"




//----------------------//
// IntervalExchangeBase //
//----------------------//



IntervalExchangeBase::IntervalExchangeBase(const std::vector<floating_point_type>& lengths,
                                           const Permutation& permutation) :
    m_permutation(permutation),
    m_inversePermutation(permutation.inverse())
{
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
        
    m_divPointsAfterExchange.resize(size);
    for (int i = 1; i < size; i++) {
        m_divPointsAfterExchange[i] = m_divPointsAfterExchange[i - 1] + m_lengths[m_inversePermutation[i - 1]];
    }
}


std::string IntervalExchangeBase::print() const{
    std::ostringstream s;
    s << "Permutation: " << m_permutation.print() << "\n";
    s << "Lengths: " << m_lengths;
    return s.str();
}


















//---------------------//
// IntervalExchangeMap //
//---------------------//



IntervalExchangeMap::IntervalExchangeMap(const std::vector<floating_point_type>& lengths, const Permutation& permutation) :
    IntervalExchangeBase(lengths, permutation),
    m_translations(lengths.size())
{
    for (unsigned int i = 0; i < lengths.size(); i++) {
        m_translations[i] = m_divPointsAfterExchange[m_permutation[i]].getPosition() - m_divPoints[i].getPosition();
    }
}




UnitIntervalPoint IntervalExchangeMap::applyTo(const UnitIntervalPoint& point) const{
    return UnitIntervalPoint(point.getPosition()) + m_translations[containingInterval(m_divPoints, point)];
}


UnitIntervalPoint IntervalExchangeMap::applyInverseTo(const UnitIntervalPoint& point) const{
    return UnitIntervalPoint(point.getPosition()) - m_translations[m_inversePermutation[containingInterval(m_divPointsAfterExchange, point)]];
}










//----------------------------//
// TwistedIntervalExchangeMap //
//----------------------------//


TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const std::vector<floating_point_type>& lengths,
                                                       const Permutation& permutation,
                                                       floating_point_type twist) :
    m_originalPermutation(permutation),
    m_twist(twist)
{
    IntervalExchangeMap originalIntExchange(lengths, permutation);
    m_originalLengths = originalIntExchange.lengths();

    unsigned int originalSize = lengths.size();
    UnitIntervalPoint oneMinusTwist(FracPart(1 - twist));
    UnitIntervalPoint newDivPoint = originalIntExchange.applyInverseTo(oneMinusTwist);
    int intervalOfNewDivPoint = containingInterval(originalIntExchange.divPoints(), newDivPoint);
    if (intervalOfNewDivPoint == CONTAINING_INTERVAL_NOT_UNIQUE) {
        throw std::runtime_error("The specified data results in an immediate saddle connection of the foliation constructed by the\
                                 twisted interval exchange map.");
    }
    unsigned int intervalOfOneMinusTwist = containingInterval(originalIntExchange.divPointsAfterExchange(), oneMinusTwist);

    // Finding the new lengths
    std::vector<floating_point_type> new_lengths = m_originalLengths;
    floating_point_type smallerLength = newDivPoint.getPosition() - originalIntExchange.divPoints()[intervalOfNewDivPoint].getPosition();
    new_lengths.insert(new_lengths.begin() + intervalOfNewDivPoint + 1, new_lengths[intervalOfNewDivPoint] - smallerLength);
    new_lengths[intervalOfNewDivPoint] = smallerLength;

    // Finding the new permutation
    std::vector<unsigned int> permutationInput(originalSize + 1);
    for (int i = 0; i < intervalOfNewDivPoint; i++) {
        if (permutation[i] < intervalOfOneMinusTwist) {
            permutationInput[i] = permutation[i] + originalSize - intervalOfOneMinusTwist;
        } else
            permutationInput[i] = permutation[i] - intervalOfOneMinusTwist;
    }
    permutationInput[intervalOfNewDivPoint] = originalSize;
    permutationInput[intervalOfNewDivPoint + 1] = 0;
    for (unsigned int i = intervalOfNewDivPoint + 2; i <= originalSize; i++) {
        if (permutation[i - 1] < intervalOfOneMinusTwist) {
            permutationInput[i] = permutation[i - 1] + originalSize - intervalOfOneMinusTwist;
        } else
            permutationInput[i] = permutation[i - 1] - intervalOfOneMinusTwist;
    }

    m_intervalExchangeAfterTwist = IntervalExchangeMap(new_lengths, Permutation(permutationInput));

    m_indexOfFakeDivPoint = m_intervalExchangeAfterTwist.inversePermutation()[0];





    /*
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
     */
}




TwistedIntervalExchangeMap TwistedIntervalExchangeMap::rotateBy(int rotationAmount) const{
    int normalizedAmount = Modint(rotationAmount, sizeBeforeTwist());
    if (normalizedAmount == 0) {
        return *this;
    }
    std::vector<floating_point_type> newLengths(sizeBeforeTwist());
    std::rotate_copy(m_originalLengths.begin(), m_originalLengths.end() - normalizedAmount, m_originalLengths.end(), newLengths.begin());

    // Postcomposing the original permutation by a rotation results in the same twisted interval exchange map if we modify the twist accordingly.
    // When the length vector is rotated, we must therefore precompose the original permutation by a rotation. By the previous remark,
    // postcomposing is not important as long as we choose the rigth twist.

    Permutation newPermutation = m_originalPermutation * rotatingPermutation(sizeBeforeTwist(), - normalizedAmount);

    int indexOfDivPointGoingToZero = m_indexOfFakeDivPoint + normalizedAmount < sizeAfterTwist() ? sizeAfterTwist() - normalizedAmount : sizeAfterTwist() - normalizedAmount - 1;
    floating_point_type rotationDistance = 1 - m_intervalExchangeAfterTwist.divPoints()[indexOfDivPointGoingToZero].getPosition();
    floating_point_type newTwist = m_twist + rotationDistance;
    
    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);
}


TwistedIntervalExchangeMap TwistedIntervalExchangeMap::reflect() const{
    std::vector<floating_point_type> newLengths(sizeBeforeTwist());
    std::reverse_copy(m_originalLengths.begin(), m_originalLengths.end(), newLengths.begin());
    
    // Now, in contract to rotateBy, we have to pre- and post-compose our premutation
    
    Permutation reverse = reversingPermutation(sizeBeforeTwist());
    Permutation newPermutation = reverse * m_originalPermutation * reverse;

    // It is not hard to see that after reflecting the whole picture, the new twist is simply the negative of the old one.
    
    floating_point_type newTwist = -m_twist;
    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);

}



TwistedIntervalExchangeMap TwistedIntervalExchangeMap::invert() const{
    std::vector<floating_point_type> newLengths(m_originalPermutation.applyAndCreateCopy(m_originalLengths));
    Permutation newPermutation = m_originalPermutation.inverse();
    floating_point_type newTwist = -m_twist;
    
    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);
}






std::ostream& operator<<(std::ostream& Out, const TwistedIntervalExchangeMap& twistedIntervalExchange){
    Out << "Lengths: " << twistedIntervalExchange.m_intervalExchangeAfterTwist.lengths() << "\n";
    Out << "Original lengths: " << twistedIntervalExchange.m_originalLengths << "\n";
    Out << "Translations: " << twistedIntervalExchange.m_intervalExchangeAfterTwist.translations() << "\n";
    
    return Out;
}


















//-------------------------------//
// IntervalExchangeFoliationDisk //
//-------------------------------//


IntervalExchangeFoliationDisk IntervalExchangeFoliationDisk::fromWeighedTree(const WeighedTree& wt)
{
    std::vector<floating_point_type> lengths(2 * wt.getNumEdges());
    std::vector<unsigned int> pairing(2 * wt.getNumEdges());
    fillInLengthsAndPairing(lengths, pairing, 0, wt.m_Root);
    return IntervalExchangeFoliationDisk(lengths, Permutation(pairing));
}



UnitIntervalPoint IntervalExchangeFoliationDisk::applyTo(const UnitIntervalPoint& point) const{
    int interval = containingInterval(m_divPoints, point);
    if (interval == CONTAINING_INTERVAL_NOT_UNIQUE) {
        throw std::runtime_error("IntervalExchangeFoliationDisk: Can't apply interval exchange, the point is very close to a division points.");
    }
    return UnitIntervalPoint(m_divPoints[m_permutation[interval]].getPosition() + m_lengths[interval] + (m_divPoints[interval].getPosition() - point.getPosition()));
}

IntervalExchangeFoliationDisk::IntervalExchangeFoliationDisk(const std::vector<floating_point_type> &lengths,
                                                             const Permutation &permutation) :
    IntervalExchangeBase(lengths, permutation)
{
}



void IntervalExchangeFoliationDisk::fillInLengthsAndPairing(std::vector<floating_point_type>& lengths,
                                                            std::vector<unsigned int>& pairing,
                                                            int StartingIndex,
                                                            WeighedTree::Node* pNode)
{
    int ChildrenStartingIndex = StartingIndex;
    for (int i = 0; i < pNode->m_NumChildren; i++) {
        int pair = ChildrenStartingIndex + 2 * pNode->m_Children[i].m_NumDescendants + 1;
        lengths[ChildrenStartingIndex] = lengths[pair] = pNode->m_Children[i].m_Weight;
        pairing[ChildrenStartingIndex] = pair;
        pairing[pair] = ChildrenStartingIndex;
        fillInLengthsAndPairing(lengths, pairing, ChildrenStartingIndex + 1, pNode->m_Children + i);
        ChildrenStartingIndex = pair + 1;
    }
}





