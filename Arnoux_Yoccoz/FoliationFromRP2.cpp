#include "FoliationFromRP2.h"




//------------------//
// FoliationFromRP2 //
//------------------//


FoliationFromRP2::FoliationFromRP2(const FoliationRP2& foliationRP2):
    FoliationFromSphere(foliationRP2),
    m_intervalPairing(IntervalExchangeFoliationDisk::fromWeighedTree(foliationRP2.m_foliationDisk.weighedTree()).permutation())
{
}





