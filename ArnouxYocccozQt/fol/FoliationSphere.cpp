#include "FoliationSphere.h"
#include "FoliationRP2.h"


balazs::FoliationSphere::FoliationSphere(const WeighedTree &topWeighedTree, const WeighedTree &bottomWeighedTree, long double twist) :
    m_topFoliation(topWeighedTree),
    m_bottomFoliation(bottomWeighedTree),
    m_twist(twist)
{
}


balazs::FoliationSphere::FoliationSphere(const FoliationRP2& foliationRP2) :
    FoliationSphere(foliationRP2.foliationDisk().weighedTree(), foliationRP2.foliationDisk().weighedTree(), 0.5)
{
}


