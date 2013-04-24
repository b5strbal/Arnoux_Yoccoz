#include "TwistedIntervalExchangeMap.h"


TwistedIntervalExchangeMap::TwistedIntervalExchangeMap() :
    IntervalExchangeBase()
{
}

TwistedIntervalExchangeMap::TwistedIntervalExchangeMap(const std::vector<floating_point_type> &lengths,
                                         const Permutation &permutation,
                                         floating_point_type twist) :
    IntervalExchangeBase(lengths, permutation, twist),
    m_translations(size())
{
    for (unsigned int i = 0; i < size(); i++) {
        m_translations[i] = m_divPointsAfterExchange[m_permutationAfterTwist[i]] - m_divPoints[i];
    }
}



Mod1Number TwistedIntervalExchangeMap::applyTo(const Mod1Number& point) const{
    return point + m_translations[containingInterval(point)];
}

Mod1NumberIntExchange TwistedIntervalExchangeMap::applyTo(const Mod1NumberIntExchange &point) const{
    assert(this == point.m_intExchange);
    return point + m_translations[containingInterval(point)];
}


Mod1Number TwistedIntervalExchangeMap::applyInverseTo(const Mod1Number& point) const{
    return point - m_translations[m_inversePermutationAfterTwist[containingIntervalAfterExchange(point)]];
}


Mod1NumberIntExchange TwistedIntervalExchangeMap::applyInverseTo(const Mod1NumberIntExchange &point) const{
    assert(this == point.m_intExchange);
    return point - m_translations[m_inversePermutationAfterTwist[containingIntervalAfterExchange(point)]];
}




TwistedIntervalExchangeMap TwistedIntervalExchangeMap::rotateBy(int rotationAmount) const{
    unsigned int normalizedAmount = Modint(rotationAmount, size());
    if (normalizedAmount == 0) {
        return *this;
    }
    std::vector<floating_point_type> newLengths(size());
    std::rotate_copy(m_lengths.begin(), m_lengths.end() - normalizedAmount, m_lengths.end(), newLengths.begin());

    // Postcomposing the original permutation by a rotation results in the same twisted interval exchange map if we modify the twist accordingly.
    // When the length vector is rotated, we must therefore precompose the original permutation by a rotation. By the previous remark,
    // postcomposing is not important as long as we choose the rigth twist.

    Permutation newPermutation = m_permutation * Permutation::rotatingPermutation(size(), - normalizedAmount);

    floating_point_type rotationDistance = 1 - m_divPoints[size() - normalizedAmount];
    floating_point_type newTwist = m_twist + rotationDistance;

    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);
}




TwistedIntervalExchangeMap TwistedIntervalExchangeMap::reverse() const{
    std::vector<floating_point_type> newLengths(size());
    std::reverse_copy(m_lengths.begin(), m_lengths.end(), m_lengths.begin());

    // Now, in contrast to rotateBy, we have to pre- and post-compose our premutation

    Permutation reverse = Permutation::reversingPermutation(size());
    Permutation newPermutation = reverse * m_permutation * reverse;

    // It is not hard to see that after reflecting the whole picture, the new twist is simply the negative of the old one.

    floating_point_type newTwist = -m_twist;
    return TwistedIntervalExchangeMap(newLengths, newPermutation, newTwist);

}



TwistedIntervalExchangeMap TwistedIntervalExchangeMap::invert() const{
    std::vector<Mod1NumberIntExchange> newLengths(m_permutation.actOn(m_lengths));
    std::vector<floating_point_type> newLengthsFloat(newLengths.begin(), newLengths.end());
    Permutation newPermutation = m_inversePermutation;
    floating_point_type newTwist = -m_twist;

    return TwistedIntervalExchangeMap(newLengthsFloat, newPermutation, newTwist);
}






std::ostream& operator<<(std::ostream& Out, const TwistedIntervalExchangeMap& twistedIntervalExchange){
    Out << "Lengths: " << twistedIntervalExchange.m_lengths << "\n";
    Out << "Permutation: " << twistedIntervalExchange.m_permutation << "\n";
    Out << "Twist:" << twistedIntervalExchange.m_twist << "\n";
    Out << "Translations: " << twistedIntervalExchange.m_translations();

    return Out;
}
