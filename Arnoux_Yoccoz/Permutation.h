#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>

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
    //! Constructs the permutation on a 1-element set.
    Permutation() : m_functionValues(1, 0) {}

    //! \brief Constructs a permutation from the list of function values.
    //! \throws std::runtime_error  If the arguments are not valid, e.g. the vector
    //!         is empty, the function values are out of range or the function
    //!         is not one-to-one.
    Permutation(const std::vector<unsigned int> &functionValues);

    //! \brief Constructs a rotating permutation. E.g. size=5, rotationAmount=2 produces {2, 3, 4, 0, 1}.
    //! \param  size                The size of the base set.
    //! \param  rotationAmount      Any integer, the amount of rotation.
    //! \throws std::runtime_error  If size<=0.
    static Permutation rotatingPermutation(int size, int rotationAmount);

    //! \brief Constructs a reversing permutation. E.g. size=5 produces {4, 3, 2, 1, 0}.
    //! \param  size    The size of the base set.
    //! \throws std::runtime_error  If size<=0.
    static Permutation reversingPermutation(int size);

    //! Returns n, the size of the base set.
    inline unsigned int size() const{ return m_functionValues.size(); }

    //! Returns the inverse permutation.
    Permutation inverse() const;

    //! \brief Acts on a vector without changing it and returns a copy.
    //! \throw std::runtime_error   If the size of the permutation and that of the vector are different.
    template <typename Type>
    std::vector<Type> actOn(const std::vector<Type>& vec) const{
        if (size() != vec.size()) {
            throw std::runtime_error("A permutation can't act on a vector if the sizes are different.");
        }
        std::vector<Type> newVector(size());
        for (unsigned int i = 0; i < size(); i++) {
            newVector[m_functionValues[i]] = vec[i];
        }
        return newVector;
    }

    //! \brief Returns the composition of the two permutations.
    //! \throws std::runtime_error  If the size of the permutations are different.
    friend Permutation operator*(const Permutation& p1, const Permutation& p2);

    //! Returns the image of element of the base set.
    unsigned int operator[](unsigned int index) const { return m_functionValues[index]; }

    //! Prints the object to a string.
    std::string print() const;

private:
    std::vector<unsigned int> m_functionValues;
};


#endif // PERMUTATION_H








