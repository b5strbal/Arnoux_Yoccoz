#include "IntervalPairing.h"



balazs::IntervalPairing balazs::IntervalPairing::fromWeighedTree(const WeighedTree& wt)
{
    std::vector<floating_point_type> lengths;
    std::vector<unsigned int> pairing;
    lengths.reserve(2 * wt.numEdges());
    pairing.reserve(2 * wt.numEdges());
    fillInLengthsAndPairing(lengths, pairing, wt.m_root);
    return IntervalPairing(lengths, Permutation(pairing));
}

std::ostream &balazs::operator <<(std::ostream &Out, const IntervalPairing &intervalPairing)
{
    Out << "Lengths: " << intervalPairing.m_intExchange.lengths() << "\n";
    Out << "Permutation: " << intervalPairing.permutation();
    return Out;
}



void balazs::IntervalPairing::fillInLengthsAndPairing(std::vector<floating_point_type>& lengths,
                                                            std::vector<unsigned int>& pairing,
                                                            WeighedTree::Node* pNode)
{
    const unsigned int PLACEHOLDER = 1986;
    for (unsigned int i = 0; i < pNode->m_numChildren; i++) {
        lengths.push_back(pNode->m_children[i].m_weight);
        unsigned int firstIndex = pairing.size();
        pairing.push_back(PLACEHOLDER);

        fillInLengthsAndPairing(lengths, pairing, pNode->m_children + i);

        lengths.push_back(pNode->m_children[i].m_weight);
        pairing[firstIndex] = pairing.size(); // replacing PLACEHOLDER by the index of the true pair
        pairing.push_back(firstIndex);
    }
}


balazs::IntervalPairing::IntervalPairing(const std::vector<balazs::floating_point_type> &lengths, const balazs::Permutation &permutation) :
    m_intExchange(lengths, permutation, 0)
{
}