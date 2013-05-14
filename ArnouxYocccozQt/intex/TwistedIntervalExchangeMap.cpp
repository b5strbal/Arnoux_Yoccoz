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

    std::cout << &m_lengthsAndTwist << "\n";

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





balazs::TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const TwistedIntervalExchangeMap& intExchange,
                                                               int rotationAmount,
                                                               const rotate_tag&)
    : TwistedIntervalExchangeMap(newLengthsRotate(intExchange, integerMod(rotationAmount, intExchange.size())),
                                 newPermutationRotate(intExchange, integerMod(rotationAmount, intExchange.size())),
                                 newTwistRotate(intExchange, integerMod(rotationAmount, intExchange.size())))
{
}



std::vector<balazs::floating_point_type>
balazs::TwistedIntervalExchangeMap::newLengthsRotate(const balazs::TwistedIntervalExchangeMap &intExchange,
                                                     int normalizedRotationAmount)
{
    if(normalizedRotationAmount == 0){
        normalizedRotationAmount = intExchange.size();
    }

    std::vector<floating_point_type> newLengths(intExchange.size());
    std::rotate_copy(intExchange.m_lengths.begin(), intExchange.m_lengths.end() - normalizedRotationAmount,
                  intExchange.m_lengths.end(), newLengths.begin());
    return newLengths;

}


balazs::Permutation
balazs::TwistedIntervalExchangeMap::newPermutationRotate(const balazs::TwistedIntervalExchangeMap &intExchange,
                                                         int normalizedRotationAmount)
{
    // Postcomposing the original permutation by a rotation results in the same twisted interval exchange map if we modify the twist accordingly.
    // When the length vector is rotated, we must therefore precompose the original permutation by a rotation. By the previous remark,
    // postcomposing is not important as long as we choose the rigth twist.

    return intExchange.m_permutation * rotatingPermutation(intExchange.size(), - normalizedRotationAmount);
}



balazs::floating_point_type
balazs::TwistedIntervalExchangeMap::newTwistRotate(const balazs::TwistedIntervalExchangeMap &intExchange,
                                                   int normalizedRotationAmount)
{
    if(normalizedRotationAmount == 0){
        return intExchange.m_twist;
    }
    floating_point_type rotationDistance = -intExchange.m_divPoints[intExchange.size() - normalizedRotationAmount];
    return static_cast<floating_point_type>(intExchange.m_twist) + rotationDistance;
}













balazs::TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const TwistedIntervalExchangeMap& intExchange,
                                                               const reverse_tag&)
    :   TwistedIntervalExchangeMap(newLengthsReverse(intExchange), newPermutationReverse(intExchange), -intExchange.m_twist)

{
}

std::vector<balazs::floating_point_type>
balazs::TwistedIntervalExchangeMap::newLengthsReverse(const balazs::TwistedIntervalExchangeMap &intExchange)
{
    std::vector<floating_point_type> newLengths(intExchange.size());
    std::reverse_copy(intExchange.m_lengths.begin(), intExchange.m_lengths.end(), newLengths.begin());
    return newLengths;
}



balazs::Permutation
balazs::TwistedIntervalExchangeMap::newPermutationReverse(const balazs::TwistedIntervalExchangeMap &intExchange)
{
    // Now, in contrast to rotateBy, we have to pre- and post-compose our premutation
    Permutation reverse = reversingPermutation(intExchange.size());
    return reverse * intExchange.m_permutation * reverse;
}



balazs::TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const TwistedIntervalExchangeMap& intExchange,
                                                               const invert_tag&) :
    TwistedIntervalExchangeMap(newLengthsInvert(intExchange), intExchange.m_inversePermutation, -intExchange.m_twist)
{
}

std::vector<balazs::floating_point_type>
balazs::TwistedIntervalExchangeMap::newLengthsInvert(const balazs::TwistedIntervalExchangeMap &intExchange)
{
    std::vector<Mod1NumberIntExchange> newLengths(intExchange.m_permutation(intExchange.m_lengths));
    return std::vector<floating_point_type>(newLengths.begin(), newLengths.end());
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



