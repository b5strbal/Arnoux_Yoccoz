/**
 * @file    IntervalExchangeMap.cpp
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the implementation for the IntervalExchangeMap class.
 */


#include "IntervalExchangeMap.h"
#include "Modint.h"

//#include "UnitIntervalPoint.h"


//----------------------//
// IntervalExchangeBase //
//----------------------//

IntervalExchangeBase::IntervalExchangeBase() : IntervalExchangeBase(std::vector<floating_point_type>(1, 1), Permutation())
{
}



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
    
    m_lengths.reserve(size);
    for (int i = 0; i < size; i++) {
        m_lengths.emplace_back(this, lengths[i]/total, i);
    }
    
    m_divPoints.resize(size);
    m_divPoints[0] = Mod1NumberIntExchange(this);
    for (int i = 1; i < size; i++) {
        m_divPoints[i] = m_divPoints[i - 1] + m_lengths[i - 1];
    }
        
    m_divPointsAfterExchange.resize(size);
    m_divPointsAfterExchange[0] = Mod1NumberIntExchange(this);
    for (int i = 1; i < size; i++) {
        m_divPointsAfterExchange[i] = m_divPointsAfterExchange[i - 1] + m_lengths[m_inversePermutation[i - 1]];
    }
}

unsigned int IntervalExchangeBase::containingInterval(const Mod1Number &point) const
{
    int interval = std::upper_bound(m_divPoints.begin(), m_divPoints.end(), point) - m_divPoints.begin() - 1;
    return interval == -1 ? size() - 1 : interval;
}

unsigned int IntervalExchangeBase::containingIntervalAfterExchange(const Mod1Number &point) const
{
    int interval = std::upper_bound(m_divPointsAfterExchange.begin(), m_divPointsAfterExchange.end(), point) - m_divPointsAfterExchange.begin() - 1;
    return interval == -1 ? size() - 1 : interval;
}


std::ostream& operator<<(std::ostream& out, const IntervalExchangeBase& exchange){
    out << "Permutation: " << exchange.m_permutation << "\n";
    out << "Lengths: " << exchange.m_lengths;
    return out;
}
















//---------------------//
// IntervalExchangeMap //
//---------------------//



IntervalExchangeMap::IntervalExchangeMap(const std::vector<floating_point_type>& lengths, const Permutation& permutation) :
    IntervalExchangeBase(lengths, permutation),
    m_translations(lengths.size())
{
    for (unsigned int i = 0; i < lengths.size(); i++) {
        m_translations[i] = m_divPointsAfterExchange[m_permutation[i]] - m_divPoints[i];
    }
}



// For applyTo and applyInverseTo, the same code works, but since they are virtual functions, we can't templatize them.

Mod1Number IntervalExchangeMap::applyTo(const Mod1Number& point) const{
    return point + m_translations[containingInterval(point)];
}

Mod1NumberIntExchange IntervalExchangeMap::applyTo(const Mod1NumberIntExchange &point) const{
    assert(this == point.m_intExchange);
    return point + m_translations[containingInterval(point)];
}


Mod1Number IntervalExchangeMap::applyInverseTo(const Mod1Number& point) const{
    return point - m_translations[m_inversePermutation[containingIntervalAfterExchange(point)]];
}


Mod1NumberIntExchange IntervalExchangeMap::applyInverseTo(const Mod1NumberIntExchange &point) const{
    assert(this == point.m_intExchange);
    return point - m_translations[m_inversePermutation[containingIntervalAfterExchange(point)]];
}










//----------------------------//
// TwistedIntervalExchangeMap //
//----------------------------//


TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const std::vector<floating_point_type>& lengths,
                                                       const Permutation& permutation,
                                                       floating_point_type twist) :
   // m_originalPermutation(permutation),
    m_twist(twist)
{
    IntervalExchangeMap originalIntExchange(lengths, permutation);
    m_originalLengths = originalIntExchange.lengths();

    unsigned int originalSize = lengths.size();
    Mod1Number oneMinusTwist(fracPart(1 - twist));
    Mod1Number newDivPoint = originalIntExchange.applyInverseTo(oneMinusTwist);
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

    Permutation newPermutation = m_originalPermutation * Permutation::rotatingPermutation(sizeBeforeTwist(), - normalizedAmount);

    int indexOfDivPointGoingToZero = m_indexOfFakeDivPoint + normalizedAmount < sizeAfterTwist() ? sizeAfterTwist() - normalizedAmount : sizeAfterTwist() - normalizedAmount - 1;
    floating_point_type rotationDistance = 1 - m_intervalExchangeAfterTwist.divPoints()[indexOfDivPointGoingToZero].getPosition();
    floating_point_type newTwist = m_twist + rotationDistance;
    
    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);
}


TwistedIntervalExchangeMap TwistedIntervalExchangeMap::reflect() const{
    std::vector<floating_point_type> newLengths(sizeBeforeTwist());
    std::reverse_copy(m_originalLengths.begin(), m_originalLengths.end(), newLengths.begin());
    
    // Now, in contract to rotateBy, we have to pre- and post-compose our premutation
    
    Permutation reverse = Permutation::reversingPermutation(sizeBeforeTwist());
    Permutation newPermutation = reverse * m_originalPermutation * reverse;

    // It is not hard to see that after reflecting the whole picture, the new twist is simply the negative of the old one.
    
    floating_point_type newTwist = -m_twist;
    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);

}



TwistedIntervalExchangeMap TwistedIntervalExchangeMap::invert() const{
    std::vector<floating_point_type> newLengths(m_originalPermutation.actOn(m_originalLengths));
    Permutation newPermutation = m_originalPermutation.inverse();
    floating_point_type newTwist = -m_twist;
    
    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);
}

Mod1Number TwistedIntervalExchangeMap::applyTo(const Mod1Number &p) const
{
}

Mod1Number TwistedIntervalExchangeMap::applyInverseTo(const Mod1Number &p) const
{
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



Mod1Number IntervalExchangeFoliationDisk::applyTo(const Mod1Number& point) const{
    int interval = containingInterval(m_divPoints, point);
    if (interval == CONTAINING_INTERVAL_NOT_UNIQUE) {
        throw std::runtime_error("IntervalExchangeFoliationDisk: Can't apply interval exchange, the point is very close to a division points.");
    }
    return Mod1Number(m_divPoints[m_permutation[interval]].getPosition() + m_lengths[interval] + (m_divPoints[interval].getPosition() - point.getPosition()));
}

Mod1NumberIntExchange IntervalExchangeFoliationDisk::applyTo(const Mod1NumberIntExchange &point) const
{
    // define
}

Mod1NumberIntExchange IntervalExchangeFoliationDisk::applyInverseTo(const Mod1NumberIntExchange &point) const
{
    //define
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





