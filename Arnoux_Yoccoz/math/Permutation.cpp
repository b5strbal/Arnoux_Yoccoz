#include "Permutation.h"
#include "../global.h"
#include <iostream>

balazs::Permutation::Permutation() : m_functionValues(1, 0)
{
}

balazs::Permutation::Permutation(const std::vector<unsigned int> &functionValues) :
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







balazs::Permutation balazs::operator*(const Permutation& p1, const Permutation& p2){
    if (p1.size() != p2.size()) {
        throw std::runtime_error("Two permutations must act on the same set in order to be composable.");
    }
    std::vector<unsigned int> compositionValues(p1.size());
    for (unsigned int i = 0 ; i < p1.size(); i++) {
        compositionValues[i] = p1[p2[i]];
    }
    return Permutation(compositionValues);
}





balazs::Permutation balazs::inverse(const Permutation& perm){
    std::vector<unsigned int> newFunctionValues(perm.size());
    for (unsigned int i = 0; i < perm.size(); i++) {
        newFunctionValues[perm[i]] = i;
    }
    return Permutation(newFunctionValues);
}



balazs::Permutation balazs::rotatingPermutation(int size, int rotationAmount){
    if (size <= 0) {
        throw std::runtime_error("Empty permutation.");
    }
    std::vector<unsigned int> functionValues(size);
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
    std::vector<unsigned int> functionValues(size);
    for (int i = 0 ; i < size; i++) {
        functionValues[i] = size - 1 - i;
    }
    return Permutation(functionValues);
}



unsigned int balazs::Permutation::size() const
{
    return m_functionValues.size();
}


std::ostream& balazs::operator <<(std::ostream &out, const Permutation &perm)
{
    for (unsigned int i = 0; i < perm.size(); i++)
        out << perm[i] << " ";
    return out;
}


bool balazs::isSimple(const Permutation &perm)
{
    for(unsigned int i = 0; i < perm.size(); i++){
        if(perm[(i + 1) % perm.size()] == (perm[i] + 1) % perm.size()){
            return false;
        }
    }
    return true;
}
