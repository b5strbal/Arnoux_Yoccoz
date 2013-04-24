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


IntervalExchangeBase::IntervalExchangeBase() :
    IntervalExchangeBase(std::vector<floating_point_type>(1, 1), Permutation(), 0)
{
}



IntervalExchangeBase::IntervalExchangeBase(const std::vector<floating_point_type>& lengths,
                                           const Permutation& permutation,
                                           floating_point_type twist) :
    m_permutation(permutation),
    m_inversePermutation(permutation.inverse()),
    m_twist(this, twist)
{
    if(lengths.size() != permutation.size()){
        throw std::runtime_error("The number of intervals is ambiguous.");
    }
    int size = lengths.size();
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
        m_lengths.push_back(Mod1NumberIntExchange(this, lengths[i]/total, i));
    }
    
    m_divPoints.resize(size);
    m_divPoints[0] = Mod1NumberIntExchange(this);
    for (int i = 1; i < size; i++) {
        m_divPoints[i] = m_divPoints[i - 1] + m_lengths[i - 1];
    }
        
    m_divPointsAfterExchange.resize(size);
    m_divPointsAfterExchange[0] = Mod1NumberIntExchange(this) + m_twist;
    for (int i = 1; i < size; i++) {
        m_divPointsAfterExchange[i] = m_divPointsAfterExchange[i - 1] + m_lengths[m_inversePermutation[i - 1]];
    }

    int shift = m_divPointsAfterExchange.end() -
            std::lower_bound(m_divPointsAfterExchange.begin(), m_divPointsAfterExchange.end(), -m_twist);
    m_permutationAfterTwist = Permutation::rotatingPermutation(size, shift) * m_permutation;
    m_inversePermutationAfterTwist = m_permutationAfterTwist.inverse();
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






















