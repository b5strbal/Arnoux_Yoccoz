#include "Permutation.h"
#include <random>
#include <cassert>

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


balazs::Permutation::Permutation(std::size_t size)
{
    assert(size > 0);

    for(std::size_t i = 0; i < size; i++){
        m_functionValues.push_back(i);
    }

    while(!isMinimal(*this)){
        std::random_shuffle(m_functionValues.begin(), m_functionValues.end());
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





bool balazs::isMinimal(const Permutation &perm)
{
    if(perm.size() == 1) return true;
    for(std::size_t i = 0; i < perm.size(); i++){
        if(perm[(i + 1) % perm.size()] == (perm[i] + 1) % perm.size()){
            return false;
        }
    }
    return true;
}



std::size_t balazs::integerMod(int a, int b){
    int result = a % b;
    return result >= 0 ? result : result + b;
}


bool balazs::operator ==(const balazs::Permutation &lhs, const balazs::Permutation &rhs)
{
    if(lhs.size() != rhs.size()){
        return false;
    }
    for(std::size_t i = 0; i < lhs.size(); i++){
        if(lhs[i] != rhs[i]){
            return false;
        }
    }
    return true;
}


bool balazs::operator !=(const balazs::Permutation &lhs, const balazs::Permutation &rhs)
{
    return !(lhs == rhs);
}
