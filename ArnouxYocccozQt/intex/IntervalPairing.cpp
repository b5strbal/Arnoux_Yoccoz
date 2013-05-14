#include "IntervalPairing.h"



balazs::IntervalPairing::IntervalPairing(const WeighedTree& wt)
    : m_intExchange(wt.getLengths(), wt.getPairing(), 0, false)
{
}

std::ostream &balazs::operator <<(std::ostream &Out, const IntervalPairing &intervalPairing)
{
    Out << "Lengths: " << intervalPairing.m_intExchange.lengths() << "\n";
    Out << "Permutation: " << intervalPairing.permutation();
    return Out;
}



