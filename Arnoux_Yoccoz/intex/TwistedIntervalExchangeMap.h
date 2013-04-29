#ifndef TWISTEDINTERVALEXCHANGEMAP_H
#define TWISTEDINTERVALEXCHANGEMAP_H

#include <iostream>
#include <vector>
#include <cassert>
#include "../math/Permutation.h"
#include "Mod1NumberIntExchange.h"

namespace balazs{




class TwistedIntervalExchangeMap{
public:
    TwistedIntervalExchangeMap(); // default constructor: the identity interval exchange on one interval.
    TwistedIntervalExchangeMap(const std::vector<floating_point_type>& lengths,
                        const Permutation& permutation,
                        floating_point_type twist);

    unsigned int size() const { return m_lengths.size(); }
    const LengthsAndTwist* signature() const { return &m_lengthsAndTwist; }
    const std::vector<Mod1NumberIntExchange>& lengths() const { return m_lengths; }
    const std::vector<Mod1NumberIntExchange>& divPoints() const { return m_divPoints; }
    const std::vector<Mod1NumberIntExchange>& divPointsAfterExchange() const { return m_divPointsAfterExchange; }
    const Permutation& permutationWithMinimalTwist() const { return m_permutation; }
    //const Permutation& inversePermutation() const { return m_inversePermutation; }
    //const std::vector<Mod1NumberIntExchange>& translations() const { return m_translations; }

    template <typename Mod1NumberType>
    Mod1NumberType applyTo(const Mod1NumberType& point) const{
        return point + m_translations[containingInterval(divPoints(), point)];
    }

    template <typename Mod1NumberType>
    Mod1NumberType applyInverseTo(const Mod1NumberType& point) const{
        return point - m_translations[m_inversePermutationWithMinimalTwist[containingInterval(divPointsAfterExchange(), point)]];
    }

    TwistedIntervalExchangeMap rotateBy(int rotationAmount) const;
    TwistedIntervalExchangeMap reverse() const;
    TwistedIntervalExchangeMap invert() const;

    //    unsigned int containingInterval(const Mod1Number& point) const;
    //    unsigned int containingIntervalAfterExchange(const Mod1Number& point) const;

    friend std::ostream& operator<<(std::ostream& Out, const TwistedIntervalExchangeMap& twistedIntervalExchange);


private:
    LengthsAndTwist m_lengthsAndTwist;
    std::vector<Mod1NumberIntExchange> m_lengths;
    Permutation m_permutation;          // Permutation of intervals before the twist.
    Permutation m_inversePermutation;   // The inverse of the above.
    Permutation m_permutationWithMinimalTwist; // Permutation of intervals after the twist.
    Permutation m_inversePermutationWithMinimalTwist; // The inverse of the above.
    Mod1NumberIntExchange m_twist;
    std::vector<Mod1NumberIntExchange> m_divPoints;
    std::vector<Mod1NumberIntExchange> m_divPointsAfterExchange;
    std::vector<Mod1NumberIntExchange> m_translations;
};

}


#endif // TWISTEDINTERVALEXCHANGEMAP_H




