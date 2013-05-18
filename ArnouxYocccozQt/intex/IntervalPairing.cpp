#include "IntervalPairing.h"
#include "../math/WeighedTree.h"



balazs::IntervalPairing::IntervalPairing(const WeighedTree& wt)
    : m_intExchange(wt.getLengths(), wt.getPairing(), 0, false)
{
}





