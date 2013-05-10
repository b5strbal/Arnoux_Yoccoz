#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>

namespace balazs{


/*!
 * \brief The Permutation class represents a permutation on integers 0, 1, ..., n-1.
 * \author Balazs Strenner, strenner@math.wisc.edu
 * \date April 19, 2013
 * \see IntervalExchangeBase, IntervalExchangeMap, TwistedIntervalExchangeMap,
 *      IntervalExchangeFoliationDisk
 *
 * As usual, a list like {3, 2, 0, 1} codes the following permutation on 4 elements:
 * 0->3, 1->2, 2->0, 3->1. If the permutation is considered to be a one-to-one
 * function {0,1,...,n-1}->{0,1,...,n-1}, then the above list is the list of the
 * values of this function at points 0, 1, ..., n-1.
 *
 * When composing two Permutations, we compose them as one usually composes two
 * functions, i.e. (p1*p2)(x) = p1(p2(x)).
 */
class Permutation{
public:
    Permutation(); // Constructs the permutation on a 1-element set.
    Permutation(const std::vector<std::size_t> &functionValues);
    std::size_t size() const;
    std::size_t operator[](std::size_t index) const { return m_functionValues[index]; }

    template <typename Type>
    const std::vector<Type> operator()(const std::vector<Type>& vec) const{
        if (size() != vec.size()) {
            throw std::runtime_error("A permutation can't act on a vector if the sizes are different.");
        }
        std::vector<Type> newVector(size());
        for (std::size_t i = 0; i < size(); i++) {
            newVector[m_functionValues[i]] = vec[i];
        }
        return newVector;
    }

private:
    std::vector<std::size_t> m_functionValues;
};


bool isMinimal(const Permutation& perm);

std::ostream& operator<<(std::ostream& out, const Permutation& perm);
Permutation operator*(const Permutation& p1, const Permutation& p2);

Permutation inverse(const Permutation& perm);
Permutation rotatingPermutation(int size, int rotationAmount);
Permutation reversingPermutation(int size);





}


#endif // PERMUTATION_H








