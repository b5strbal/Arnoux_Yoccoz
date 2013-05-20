#include "FoliationFromRP2.h"
#include "FoliationRP2.h"
#include "FoliationSphere.h"

balazs::FoliationFromRP2::FoliationFromRP2(const FoliationRP2& foliationRP2) :
    FoliationFromSphere((FoliationSphere(foliationRP2))),
    m_intervalPermutationBeforeHalfTwist(foliationRP2.foliationDisk().weighedTree().getPairing())
{
}


const balazs::Permutation &balazs::FoliationFromRP2::intervalPermutationBeforeHalfTwist() const
{
    return m_intervalPermutationBeforeHalfTwist;
}


bool balazs::comesFromRP2(const balazs::Foliation &foliation)
{
    return dynamic_cast<const balazs::FoliationFromRP2*>(&foliation) != nullptr;
}
