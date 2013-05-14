#include "FoliationSphere.h"
#include "FoliationRP2.h"


balazs::FoliationSphere::FoliationSphere(const WeighedTree &topWeighedTree, const WeighedTree &bottomWeighedTree, floating_point_type twist) :
    m_topFoliation(topWeighedTree),
    m_bottomFoliation(bottomWeighedTree),
    m_twist(twist)
{
}


balazs::FoliationSphere::FoliationSphere(const FoliationRP2& foliationRP2) :
    FoliationSphere(foliationRP2.foliationDisk().weighedTree(), foliationRP2.foliationDisk().weighedTree(), 0.5)
{
}


std::ostream& balazs::operator<<(std::ostream& Out, const FoliationSphere& f){
    Out << "(Top Disk) " << f.topFoliation() << std::endl;
    Out << "(Bottom Disk) " << f.bottomFoliation() << std::endl;
    Out << "Twist: " << f.twist();

    return Out;
}


