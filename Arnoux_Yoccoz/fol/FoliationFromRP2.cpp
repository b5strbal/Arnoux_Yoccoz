#include "FoliationFromRP2.h"



balazs::FoliationFromRP2::FoliationFromRP2(const FoliationRP2& foliationRP2):
    FoliationFromSphere(foliationRP2),
    m_intervalPairing(IntervalPairing::fromWeighedTree(foliationRP2.m_foliationDisk.weighedTree()).permutation())
{
}





