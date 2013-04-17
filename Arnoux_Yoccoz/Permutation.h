#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>

class Permutation{
public:
    Permutation() : m_functionValues(1, 0) {} // trivial permutation on one element
    Permutation(const std::vector<unsigned int> &functionValues);
    inline unsigned int size() const{ return m_functionValues.size(); }
    Permutation inverse() const;

    template <typename Type>
    std::vector<Type> applyAndCreateCopy(const std::vector<Type>& vec) const;

    friend Permutation operator*(const Permutation& p1, const Permutation& p2);
    int operator[](int index) const { return m_functionValues[index]; }
    std::string print() const;

private:
    std::vector<unsigned int> m_functionValues;
};

Permutation rotatingPermutation(int size, int rotationAmount);
Permutation reversingPermutation(int size);
#endif // PERMUTATION_H
