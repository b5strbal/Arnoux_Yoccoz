#ifndef SMALLFOLIATION_H
#define SMALLFOLIATION_H

#include <vector>
#include "../intex/Mod1NumberIntExchange.h"
#include "../math/Permutation.h"

namespace balazs{

class TransverseCurve;

class SmallFoliation
{
public:
    SmallFoliation(const TransverseCurve& tc, std::size_t referenceZeroIndex, bool flippedOver, bool orientationReversing);


    const std::vector<long double>& normalizedLengths() const { return m_normalizedLengths; }
    const Permutation& permutationWithMinimalTwist() const { return m_permutation; }
    long double normalizedTwist() const { return m_normalizedTwist; }

private:
    std::vector<Mod1NumberIntExchange> m_lengths;
    std::vector<long double> m_normalizedLengths;
    Permutation m_permutation;
    Mod1NumberIntExchange m_twist;
    long double m_normalizedTwist;
    Mod1NumberIntExchange m_totalLength;
//    Permutation m_singularityPermutation;
};

}

#endif // SMALLFOLIATION_H
