#include "Permutation.h"
#include "Modint.h"
#include <string>
#include <sstream>


Permutation::Permutation() : m_functionValues(1, 0) {}
{
}

Permutation::Permutation(const std::vector<unsigned int> &functionValues) :
    m_functionValues(functionValues)
{
    if (size() == 0) throw std::runtime_error("Empty permutation.");
    std::vector<char> isInTheRange(size(), 0);
    for (unsigned int i = 0; i < size(); i++) {
        if(m_functionValues[i] >= size())
            throw std::runtime_error("A permutation must be specified by integers between 0 and n-1.");
        if (isInTheRange[m_functionValues[i]] == 1)
            throw std::runtime_error("A permutation should be a one-to-one map.");
        isInTheRange[m_functionValues[i]] = 1;
    }
}







Permutation operator*(const Permutation& p1, const Permutation& p2){
    if (p1.size() != p2.size()) {
        throw std::runtime_error("Two permutations must act on the same set in order to be composable.");
    }
    std::vector<unsigned int> compositionValues(p1.size());
    for (unsigned int i = 0 ; i < p1.size(); i++) {
        compositionValues[i] = p1[p2[i]];
    }
    return Permutation(compositionValues);
}





Permutation Permutation::inverse() const{
    std::vector<unsigned int> newFunctionValues(size());
    for (unsigned int i = 0; i < size(); i++) {
        newFunctionValues[m_functionValues[i]] = i;
    }
    return Permutation(newFunctionValues);
}



Permutation rotatingPermutation(int size, int rotationAmount){
    if (size <= 0) {
        throw std::runtime_error("Empty permutation.");
    }
    std::vector<unsigned int> functionValues(size);
    int normalizedAmount = Modint(rotationAmount, size);
    for (int i = 0; i < size - normalizedAmount; i++) {
        functionValues[i] = i + normalizedAmount;
    }
    for (int i = size - normalizedAmount; i < size; i++) {
        functionValues[i] = i + normalizedAmount - size;
    }
    return Permutation(functionValues);
}


Permutation reversingPermutation(int size){
    if (size <= 0) {
        throw std::runtime_error("Empty permutation.");
    }
    std::vector<unsigned int> functionValues(size);
    for (int i = 0 ; i < size; i++) {
        functionValues[i] = size - 1 - i;
    }
    return Permutation(functionValues);
}



unsigned int Permutation::size() const
{
    return m_functionValues.size();
}


std::ostream &Permutation::operator <<(std::ostream &out, const Permutation &perm) const
{
    for (unsigned int i = 0; i < perm.size(); i++)
        out << perm.m_functionValues[i] << " ";
    return out;
}
