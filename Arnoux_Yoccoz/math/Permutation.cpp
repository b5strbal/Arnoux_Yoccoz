#include "Permutation.h"
#include "../global.h"
#include <iostream>

balazs::Permutation::Permutation() : m_functionValues(1, 0)
{
}

balazs::Permutation::Permutation(const std::vector<std::size_t> &functionValues) :
    m_functionValues(functionValues)
{
    if (size() == 0) throw std::runtime_error("Empty permutation.");
    std::vector<char> isInTheRange(size(), 0);
    for (std::size_t i = 0; i < size(); i++) {
        if(m_functionValues[i] >= size())
            throw std::runtime_error("A permutation must be specified by integers between 0 and n-1.");
        if (isInTheRange[m_functionValues[i]] == 1)
            throw std::runtime_error("A permutation should be a one-to-one map.");
        isInTheRange[m_functionValues[i]] = 1;
    }
}







balazs::Permutation balazs::operator*(const Permutation& p1, const Permutation& p2){
    if (p1.size() != p2.size()) {
        throw std::runtime_error("Two permutations must act on the same set in order to be composable.");
    }
    std::vector<std::size_t> compositionValues(p1.size());
    for (std::size_t i = 0 ; i < p1.size(); i++) {
        compositionValues[i] = p1[p2[i]];
    }
    return Permutation(compositionValues);
}





balazs::Permutation balazs::inverse(const Permutation& perm){
    std::vector<std::size_t> newFunctionValues(perm.size());
    for (std::size_t i = 0; i < perm.size(); i++) {
        newFunctionValues[perm[i]] = i;
    }
    return Permutation(newFunctionValues);
}



balazs::Permutation balazs::rotatingPermutation(int size, int rotationAmount){
    if (size <= 0) {
        throw std::runtime_error("Empty permutation.");
    }
    std::vector<std::size_t> functionValues(size);
    int normalizedAmount = integerMod(rotationAmount, size);
    for (int i = 0; i < size - normalizedAmount; i++) {
        functionValues[i] = i + normalizedAmount;
    }
    for (int i = size - normalizedAmount; i < size; i++) {
        functionValues[i] = i + normalizedAmount - size;
    }
    return Permutation(functionValues);
}


balazs::Permutation balazs::reversingPermutation(int size){
    if (size <= 0) {
        throw std::runtime_error("Empty permutation.");
    }
    std::vector<std::size_t> functionValues(size);
    for (int i = 0 ; i < size; i++) {
        functionValues[i] = size - 1 - i;
    }
    return Permutation(functionValues);
}



std::size_t balazs::Permutation::size() const
{
    return m_functionValues.size();
}


std::ostream& balazs::operator <<(std::ostream &out, const Permutation &perm)
{
    for (std::size_t i = 0; i < perm.size(); i++)
        out << perm[i] << " ";
    return out;
}


bool balazs::isSimple(const Permutation &perm)
{
    for(std::size_t i = 0; i < perm.size(); i++){
        if(perm[(i + 1) % perm.size()] == (perm[i] + 1) % perm.size()){
            return false;
        }
    }
    return true;
}
