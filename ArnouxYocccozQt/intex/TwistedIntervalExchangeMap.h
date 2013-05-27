#ifndef TWISTEDINTERVALEXCHANGEMAP_H
#define TWISTEDINTERVALEXCHANGEMAP_H

#include "../math/Permutation.h"
#include "Mod1NumberIntExchange.h"
//#include "LengthsAndTwist.h"
#include "ContainingInterval.h"

namespace balazs{


struct rotate_tag {};
struct reverse_tag {};
struct invert_tag {};

class TwistedIntervalExchangeMap{
public:
   // TwistedIntervalExchangeMap(); // default constructor: the identity interval exchange on one interval.
    TwistedIntervalExchangeMap(const std::vector<long double>& lengths,
                        const Permutation& permutation,
                        long double twist, bool permutationMustBeMinimal = true);
    TwistedIntervalExchangeMap(const TwistedIntervalExchangeMap&) = delete;
    TwistedIntervalExchangeMap& operator=(const TwistedIntervalExchangeMap&) = delete;

    std::size_t size() const { return m_lengths.size(); }
    const std::vector<Mod1NumberIntExchange>& lengths() const { return m_lengths; }
    const Mod1NumberIntExchange& twist() const { return m_twist; }
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


private:
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

std::vector<long double> newLengthsRotate(const TwistedIntervalExchangeMap& intExchange,
                                                  int normalizedRotationAmount);
std::vector<long double> newLengthsReverse(const TwistedIntervalExchangeMap& intExchange);
std::vector<long double> newLengthsInvert(const TwistedIntervalExchangeMap& intExchange);

Permutation newPermutationRotate(const TwistedIntervalExchangeMap& intExchange,
                                                  int normalizedRotationAmount);
Permutation newPermutationReverse(const TwistedIntervalExchangeMap& intExchange);

long double newTwistRotate(const TwistedIntervalExchangeMap& intExchange, int normalizedRotationAmount);



}


#endif // TWISTEDINTERVALEXCHANGEMAP_H




