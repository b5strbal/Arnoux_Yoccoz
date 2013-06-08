#ifndef INTERVALPAIRING_H
#define INTERVALPAIRING_H

#include "TwistedIntervalExchangeMap.h"

namespace balazs{


class WeighedTree;

class IntervalPairing
{
public:
    IntervalPairing(const WeighedTree& wt);
    std::size_t size() const { return m_intExchange.size(); }
    const std::vector<Mod1NumberIntExchange>& divPoints() const { return m_intExchange.divPoints(); }
    const Permutation& permutation() const { return m_intExchange.permutationWithMinimalTwist(); }
    const std::vector<Mod1NumberIntExchange>& lengths() const { return m_intExchange.lengths(); }

    template <typename Mod1NumberType>
    Mod1NumberType applyTo(const Mod1NumberType& point) const{
        std::size_t intervalIndex = containingInterval(m_intExchange.divPoints(), point);
        return m_intExchange.divPoints()[permutation()[intervalIndex]] + m_intExchange.lengths()[intervalIndex] +
                (m_intExchange.divPoints()[intervalIndex] - point);
    }

private:
    TwistedIntervalExchangeMap m_intExchange;
};


Permutation singularityPermutation(const IntervalPairing& ip);


}




#endif // INTERVALPAIRING_H
