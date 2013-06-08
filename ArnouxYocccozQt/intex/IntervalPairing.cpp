#include "IntervalPairing.h"
#include "../math/WeighedTree.h"
#include "math/Permutation.h"


balazs::IntervalPairing::IntervalPairing(const WeighedTree& wt)
    : m_intExchange(wt.getLengths(), wt.getPairing(), 0, false)
{
}



balazs::Permutation balazs::singularityPermutation(const IntervalPairing& ip){
    std::vector<std::size_t> permutationInput(ip.size());
    for(std::size_t i = 0; i < ip.size(); i++){
        permutationInput[i] = (ip.permutation()[i] + 1) % ip.size();
    }
    return Permutation(permutationInput);
}


