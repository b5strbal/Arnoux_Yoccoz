#include "TwistedIntervalExchangeMap.h"




balazs::TwistedIntervalExchangeMap::TwistedIntervalExchangeMap() :
    TwistedIntervalExchangeMap(std::vector<floating_point_type>(1, 1), Permutation(), 0)
{
}

balazs::TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const std::vector<floating_point_type> &lengths,
                                         const Permutation &permutation,
                                         floating_point_type twist, bool permutationMustBeMinimal) :
    m_lengthsAndTwist(lengths, twist),
    m_permutation(permutation),
    m_inversePermutation(inverse(permutation)),
    m_twist(Mod1NumberIntExchange::constructTwist(&m_lengthsAndTwist))
{
    if(lengths.size() != permutation.size()){
        throw std::runtime_error("The size of the permutation and the number of intervals should be the same.");
    }

    if(permutationMustBeMinimal){
        if(!isMinimal(permutation)){
            throw std::runtime_error("The interval exchange map is not \"minimal\", i.e. the same map can be specified with smaller number of number of intervals.");
        }
    }

    m_lengths.reserve(lengths.size());
    for (std::size_t i = 0; i < lengths.size(); i++) {
        m_lengths.push_back(Mod1NumberIntExchange::constructLength(&m_lengthsAndTwist, i));
    }

    m_divPoints.resize(lengths.size());
    m_divPoints[0] = Mod1NumberIntExchange::constructZero(&m_lengthsAndTwist);
    for (std::size_t i = 1; i < lengths.size(); i++) {
        m_divPoints[i] = m_divPoints[i - 1] + m_lengths[i - 1];
    }

    m_divPointsAfterExchange.resize(lengths.size());
    m_divPointsAfterExchange[0] = m_twist;
    int divpointsBeforeTwist = 0;
    for (std::size_t i = 1; i < lengths.size(); i++) {
        m_divPointsAfterExchange[i] = m_divPointsAfterExchange[i - 1] + m_lengths[m_inversePermutation[i - 1]];
        if(static_cast<floating_point_type>(m_divPointsAfterExchange[i - 1]) +
                static_cast<floating_point_type>(m_lengths[m_inversePermutation[i - 1]]) > 1){
            divpointsBeforeTwist = lengths.size() - i;
        }
    }
    std::sort(m_divPointsAfterExchange.begin(), m_divPointsAfterExchange.end());

    m_permutationWithMinimalTwist = rotatingPermutation(lengths.size(), divpointsBeforeTwist) * m_permutation;
    m_inversePermutationWithMinimalTwist = inverse(m_permutationWithMinimalTwist);

    m_translations.reserve(lengths.size());
    for (std::size_t i = 0; i < lengths.size(); i++) {
        m_translations.push_back(m_divPointsAfterExchange[m_permutationWithMinimalTwist[i]] - m_divPoints[i]);
    }
}







balazs::TwistedIntervalExchangeMap balazs::TwistedIntervalExchangeMap::rotateBy(int rotationAmount) const{
    std::size_t normalizedAmount = integerMod(rotationAmount, size());
    if (normalizedAmount == 0) {
        return *this;
    }
    std::vector<floating_point_type> newLengths(size());
    std::rotate_copy(m_lengths.begin(), m_lengths.end() - normalizedAmount, m_lengths.end(), newLengths.begin());

    // Postcomposing the original permutation by a rotation results in the same twisted interval exchange map if we modify the twist accordingly.
    // When the length vector is rotated, we must therefore precompose the original permutation by a rotation. By the previous remark,
    // postcomposing is not important as long as we choose the rigth twist.

    Permutation newPermutation = m_permutation * rotatingPermutation(size(), - normalizedAmount);

    floating_point_type rotationDistance = -m_divPoints[size() - normalizedAmount];
    floating_point_type newTwist = static_cast<floating_point_type>(m_twist) + rotationDistance;

    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);
}




balazs::TwistedIntervalExchangeMap balazs::TwistedIntervalExchangeMap::reverse() const{
    std::vector<floating_point_type> newLengths(size());
    std::reverse_copy(m_lengths.begin(), m_lengths.end(), newLengths.begin());

    // Now, in contrast to rotateBy, we have to pre- and post-compose our premutation

    Permutation reverse = reversingPermutation(size());
    Permutation newPermutation = reverse * m_permutation * reverse;

    // It is not hard to see that after reflecting the whole picture, the new twist is simply the negative of the old one.

    floating_point_type newTwist = -m_twist;
    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);

}



balazs::TwistedIntervalExchangeMap balazs::TwistedIntervalExchangeMap::invert() const{
    std::vector<Mod1NumberIntExchange> newLengths(m_permutation(m_lengths));
    std::vector<floating_point_type> newLengthsFloat(newLengths.begin(), newLengths.end());
    Permutation newPermutation = m_inversePermutation;
    floating_point_type newTwist = -m_twist;

    return TwistedIntervalExchangeMap(newLengthsFloat, newPermutation, newTwist);
}




std::ostream& balazs::operator<<(std::ostream& Out, const TwistedIntervalExchangeMap& twistedIntervalExchange){
    Out << "Lengths: " << twistedIntervalExchange.m_lengths << "\n";
    Out << "Permutation: " << twistedIntervalExchange.m_permutation << "\n";
    Out << "Twist:" << twistedIntervalExchange.m_twist << "\n";
    Out << "Translations: " << twistedIntervalExchange.m_translations;

    return Out;
}






//std::size_t IntervalExchangeBase::containingInterval(const Mod1Number &point) const
//{
////    int interval = std::upper_bound(m_divPoints.begin(), m_divPoints.end(), point) - m_divPoints.begin() - 1;
////    return interval == -1 ? size() - 1 : interval;
//    return ::containingInterval(m_divPoints, point);
//}

//std::size_t IntervalExchangeBase::containingIntervalAfterExchange(const Mod1Number &point) const
//{
//    int interval = std::upper_bound(m_divPointsAfterExchange.begin(), m_divPointsAfterExchange.end(), point) - m_divPointsAfterExchange.begin() - 1;
//    return interval == -1 ? size() - 1 : interval;
//}

