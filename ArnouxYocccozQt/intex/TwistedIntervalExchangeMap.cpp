#include "TwistedIntervalExchangeMap.h"
#include "ContainingInterval.h"
#include <numeric>


//balazs::TwistedIntervalExchangeMap::TwistedIntervalExchangeMap() :
//    TwistedIntervalExchangeMap(std::vector<long double>(1, 1), Permutation(), 0)
//{
//}

balazs::TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const std::vector<long double> &lengths,
                                         const Permutation &permutation,
                                         long double twist, bool permutationMustBeMinimal) :
    m_permutation(permutation),
    m_inversePermutation(inverse(permutation))
{
    if(lengths.size() != permutation.size()){
        throw std::runtime_error("The size of the permutation and the number of intervals should be the same.");
    }

    if(permutationMustBeMinimal){
        if(!isMinimal(permutation)){
            throw std::runtime_error("The interval exchange map is not \"minimal\", i.e. the same map can be specified with smaller number of number of intervals.");
        }
    }

    if (lengths.size() == 0){
        throw std::runtime_error("The number of intervals must be at least 1.");
    }
    for (std::size_t i = 0; i < lengths.size(); i++) {
        if (lengths[i] <= 0)
            throw std::runtime_error("The length parameters of an interval exchange map must be positive.");
    }

    long double total = std::accumulate(lengths.begin(), lengths.end(), 0.0);




    m_lengths.reserve(lengths.size());
    std::vector<int> coefficients;
    coefficients.resize(lengths.size());
    for (std::size_t i = 0; i < lengths.size(); i++) {
        coefficients[i] = 1;
        m_lengths.push_back(Mod1NumberIntExchange(this, lengths[i] / total, coefficients, 0));
        coefficients[i] = 0;
    }


    m_divPoints.reserve(lengths.size());
    m_divPoints.push_back(Mod1NumberIntExchange(this, 0, coefficients, 0));
    for (std::size_t i = 1; i < lengths.size(); i++) {
        m_divPoints.push_back(m_divPoints[i - 1] + m_lengths[i - 1]);
    }


    std::vector<Mod1Number> divPointsAfter;
    divPointsAfter.resize(lengths.size());
    divPointsAfter[0] = twist;
    int divpointsBeforeTwist = 0;
    for (std::size_t i = 1; i < lengths.size(); i++) {
        divPointsAfter[i] = divPointsAfter[i - 1] + m_lengths[m_inversePermutation[i - 1]];
        if(static_cast<long double>(divPointsAfter[i - 1]) +
                static_cast<long double>(m_lengths[m_inversePermutation[i - 1]]) > 1){
            divpointsBeforeTwist = lengths.size() - i;
        }
    }
    m_twist = Mod1NumberIntExchange(this, *std::min_element(divPointsAfter.begin(),
                                                   divPointsAfter.end()), coefficients, 1);


    m_permutationWithMinimalTwist = rotatingPermutation(lengths.size(), divpointsBeforeTwist) * m_permutation;
    m_inversePermutationWithMinimalTwist = inverse(m_permutationWithMinimalTwist);


    m_divPointsAfterExchange.resize(lengths.size());
    m_divPointsAfterExchange[0] = m_twist;
    for (std::size_t i = 1; i < lengths.size(); i++) {
        m_divPointsAfterExchange[i] = m_divPointsAfterExchange[i - 1] +
                m_lengths[m_inversePermutationWithMinimalTwist[i - 1]];
    }
    std::sort(m_divPointsAfterExchange.begin(), m_divPointsAfterExchange.end());



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



std::vector<long double>
balazs::TwistedIntervalExchangeMap::newLengthsRotate(const balazs::TwistedIntervalExchangeMap &intExchange,
                                                     int normalizedRotationAmount)
{
    if(normalizedRotationAmount == 0){
        normalizedRotationAmount = intExchange.size();
    }

    std::vector<long double> newLengths(intExchange.size());
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



long double
balazs::TwistedIntervalExchangeMap::newTwistRotate(const balazs::TwistedIntervalExchangeMap &intExchange,
                                                   int normalizedRotationAmount)
{
    if(normalizedRotationAmount == 0){
        return intExchange.m_twist;
    }
    long double rotationDistance = -intExchange.m_divPoints[intExchange.size() - normalizedRotationAmount];
    return static_cast<long double>(intExchange.m_twist) + rotationDistance;
}













balazs::TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const TwistedIntervalExchangeMap& intExchange,
                                                               const reverse_tag&)
    :   TwistedIntervalExchangeMap(newLengthsReverse(intExchange), newPermutationReverse(intExchange), -intExchange.m_twist)

{
}

std::vector<long double>
balazs::TwistedIntervalExchangeMap::newLengthsReverse(const balazs::TwistedIntervalExchangeMap &intExchange)
{
    std::vector<long double> newLengths(intExchange.size());
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

std::vector<long double>
balazs::TwistedIntervalExchangeMap::newLengthsInvert(const balazs::TwistedIntervalExchangeMap &intExchange)
{
    std::vector<Mod1NumberIntExchange> newLengths(intExchange.m_permutation(intExchange.m_lengths));
    return std::vector<long double>(newLengths.begin(), newLengths.end());
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



