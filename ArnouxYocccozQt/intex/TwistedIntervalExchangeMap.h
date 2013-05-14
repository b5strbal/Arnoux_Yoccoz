#ifndef TWISTEDINTERVALEXCHANGEMAP_H
#define TWISTEDINTERVALEXCHANGEMAP_H

#include <iostream>
#include <vector>
#include <cassert>
#include "../math/Permutation.h"
#include "Mod1NumberIntExchange.h"
#include "LengthsAndTwist.h"

namespace balazs{


struct rotate_tag {};
struct reverse_tag {};
struct invert_tag {};

class TwistedIntervalExchangeMap{
public:
    TwistedIntervalExchangeMap(); // default constructor: the identity interval exchange on one interval.
    TwistedIntervalExchangeMap(const std::vector<floating_point_type>& lengths,
                        const Permutation& permutation,
                        floating_point_type twist, bool permutationMustBeMinimal = true);
    TwistedIntervalExchangeMap(const TwistedIntervalExchangeMap&) = delete;
    TwistedIntervalExchangeMap& operator=(const TwistedIntervalExchangeMap&) = delete;

    std::size_t size() const { return m_lengths.size(); }
    const LengthsAndTwist* signature() const { return &m_lengthsAndTwist; }
    const std::vector<Mod1NumberIntExchange>& lengths() const { return m_lengths; }
    const std::vector<Mod1NumberIntExchange>& divPoints() const { return m_divPoints; }
    const std::vector<Mod1NumberIntExchange>& divPointsAfterExchange() const { return m_divPointsAfterExchange; }
    const Permutation& permutationWithMinimalTwist() const { return m_permutationWithMinimalTwist; }
    const Permutation& inversePermutationWithMinimalTwist() const { return m_inversePermutationWithMinimalTwist; }

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
    TwistedIntervalExchangeMap(const TwistedIntervalExchangeMap& intExchange, int rotationAmount, const rotate_tag&);
    TwistedIntervalExchangeMap(const TwistedIntervalExchangeMap& intExchange, const reverse_tag&);
    TwistedIntervalExchangeMap(const TwistedIntervalExchangeMap& intExchange, const invert_tag&);

    //    std::size_t containingInterval(const Mod1Number& point) const;
    //    std::size_t containingIntervalAfterExchange(const Mod1Number& point) const;

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


    static std::vector<floating_point_type> newLengthsRotate(const TwistedIntervalExchangeMap& intExchange,
                                                      int normalizedRotationAmount);
    static std::vector<floating_point_type> newLengthsReverse(const TwistedIntervalExchangeMap& intExchange);
    static std::vector<floating_point_type> newLengthsInvert(const TwistedIntervalExchangeMap& intExchange);

    static Permutation newPermutationRotate(const TwistedIntervalExchangeMap& intExchange,
                                                      int normalizedRotationAmount);
    static Permutation newPermutationReverse(const TwistedIntervalExchangeMap& intExchange);


    static floating_point_type newTwistRotate(const TwistedIntervalExchangeMap& intExchange, int normalizedRotationAmount);


};

}


#endif // TWISTEDINTERVALEXCHANGEMAP_H




