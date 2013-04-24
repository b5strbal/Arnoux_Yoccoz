
#ifndef INTERVALEXCHANGEBASE_H
#define INTERVALEXCHANGEBASE_H

#include <iostream>
#include <vector>
#include <cassert>
#include "Mod1Number.h"
#include "global.h"
#include "Permutation.h"
#include "Mod1NumberIntExchange.h"





class Mod1NumberIntExchange;

            
class IntervalExchangeBase{
public:
    unsigned int size() const { return m_lengths.size(); }
    virtual Mod1Number applyTo(const Mod1Number& point) const = 0;
    virtual Mod1NumberIntExchange applyTo(const Mod1NumberIntExchange &point) const = 0;
    virtual Mod1Number applyInverseTo(const Mod1Number& point) const = 0;
    virtual Mod1NumberIntExchange applyInverseTo(const Mod1NumberIntExchange &point) const = 0;
    //const std::vector<Mod1NumberIntExchange>& lengths() const { return m_lengths; }
    //const Permutation& permutation() const { return m_permutation; }
    //const Permutation& inversePermutation() const { return m_inversePermutation; }
    //const std::vector<Mod1NumberIntExchange>& divPoints() const { return m_divPoints; }
    //const std::vector<Mod1NumberIntExchange>& divPointsAfterExchange() const { return m_divPointsAfterExchange; }
    friend std::ostream& operator<<(std::ostream& out, const IntervalExchangeBase& exchange);

protected:
    std::vector<Mod1NumberIntExchange> m_lengths;
    Permutation m_permutation;          // Permutation of intervals before the twist.
    Permutation m_inversePermutation;   // The inverse of the above.
    Permutation m_permutationAfterTwist; // Permutation of intervals after the twist.
    Permutation m_inversePermutationAfterTwist; // The inverse of the above.
    Mod1NumberIntExchange m_twist;
    std::vector<Mod1NumberIntExchange> m_divPoints;
    std::vector<Mod1NumberIntExchange> m_divPointsAfterExchange;

    IntervalExchangeBase(); // default constructor: the identity interval exchange on one interval.
    IntervalExchangeBase(const std::vector<floating_point_type>& lengths,
                         const Permutation& permutation,
                         floating_point_type twist);

    unsigned int containingInterval(const Mod1Number& point) const;
    unsigned int containingIntervalAfterExchange(const Mod1Number& point) const;
};







#endif
