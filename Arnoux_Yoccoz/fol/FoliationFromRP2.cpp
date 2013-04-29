#include "FoliationFromRP2.h"



balazs::FoliationFromRP2::FoliationFromRP2(const FoliationRP2& foliationRP2):
    FoliationFromSphere(FoliationFromSphere::fromFoliationSphere(foliationRP2)),
    m_intervalPermutationBeforeHalfTwist(IntervalPairing::fromWeighedTree(foliationRP2.foliationDisk().weighedTree()).permutation())
{
}


const balazs::Permutation &balazs::FoliationFromRP2::intervalPermutationBeforeHalfTwist() const
{
    return m_intervalPermutationBeforeHalfTwist;
}




