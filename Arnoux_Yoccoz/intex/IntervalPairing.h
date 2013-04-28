#ifndef INTERVALPAIRING_H
#define INTERVALPAIRING_H

#include "TwistedIntervalExchangeMap.h"
#include "../math/WeighedTree.h"

namespace balazs{



class IntervalPairing
{
public:
    static IntervalPairing fromWeighedTree(const WeighedTree& wt);
    unsigned int size() const { return m_intExchange.size(); }
    std::vector<Mod1NumberIntExchange> divPoints() const { return m_intExchange.divPoints(); }
    const Permutation& permutation() const { return m_intExchange.permutationWithMinimalTwist(); }

    template <typename Mod1NumberType>
    Mod1NumberType applyTo(const Mod1NumberType& point) const{
        unsigned int intervalIndex = containingInterval(m_intExchange.divPoints(), point);
        return m_intExchange.divPoints()[permutation()[intervalIndex]] + m_intExchange.lengths()[intervalIndex] +
                (m_intExchange.divPoints()[intervalIndex] - point);
    }

    friend std::ostream& operator<<(std::ostream& Out, const IntervalPairing& intervalPairing);
private:
    IntervalPairing(const std::vector<floating_point_type>& lengths, const Permutation& permutation);

    static void fillInLengthsAndPairing(std::vector<floating_point_type>& lengths,
                                 std::vector<unsigned int> &pairing,
                                 int StartingIndex,
                                 WeighedTree::Node* pNode);


    TwistedIntervalExchangeMap m_intExchange;
};

}


#endif // INTERVALPAIRING_H
