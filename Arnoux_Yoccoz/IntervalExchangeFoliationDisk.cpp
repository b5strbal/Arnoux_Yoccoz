#include "IntervalExchangeFoliationDisk.h"



IntervalExchangeFoliationDisk IntervalExchangeFoliationDisk::fromWeighedTree(const WeighedTree& wt)
{
    std::vector<floating_point_type> lengths(2 * wt.getNumEdges());
    std::vector<unsigned int> pairing(2 * wt.getNumEdges());
    fillInLengthsAndPairing(lengths, pairing, 0, wt.m_Root);
    return IntervalExchangeFoliationDisk(lengths, Permutation(pairing));
}



Mod1Number IntervalExchangeFoliationDisk::applyTo(const Mod1Number& point) const{
    int interval = Mod1Number::containingInterval(m_divPoints, point);
    return m_divPoints[m_permutation[interval]] + m_lengths[interval] + (m_divPoints[interval] - point);
}

Mod1NumberIntExchange IntervalExchangeFoliationDisk::applyTo(const Mod1NumberIntExchange &point) const
{
    int interval = Mod1Number::containingInterval(m_divPoints, point);
    return m_divPoints[m_permutation[interval]] + m_lengths[interval] + (m_divPoints[interval] - point);
}

Mod1NumberIntExchange IntervalExchangeFoliationDisk::applyInverseTo(const Mod1NumberIntExchange &point) const
{
    return applyInverseTo(point);
}




void IntervalExchangeFoliationDisk::fillInLengthsAndPairing(std::vector<floating_point_type>& lengths,
                                                            std::vector<unsigned int>& pairing,
                                                            int StartingIndex,
                                                            WeighedTree::Node* pNode)
{
    int ChildrenStartingIndex = StartingIndex;
    for (int i = 0; i < pNode->m_NumChildren; i++) {
        int pair = ChildrenStartingIndex + 2 * pNode->m_Children[i].m_NumDescendants + 1;
        lengths[ChildrenStartingIndex] = lengths[pair] = pNode->m_Children[i].m_Weight;
        pairing[ChildrenStartingIndex] = pair;
        pairing[pair] = ChildrenStartingIndex;
        fillInLengthsAndPairing(lengths, pairing, ChildrenStartingIndex + 1, pNode->m_Children + i);
        ChildrenStartingIndex = pair + 1;
    }
}
