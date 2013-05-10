#include "FoliationSphere.h"

balazs::FoliationSphere::FoliationSphere(const FoliationDisk& topFoliation, const FoliationDisk& bottomFoliation, floating_point_type twist) :
    m_topFoliation(topFoliation),
    m_bottomFoliation(bottomFoliation),
    m_twist(twist)
{
}


balazs::FoliationSphere::FoliationSphere(const FoliationRP2& foliationRP2) :
    FoliationSphere(foliationRP2.foliationDisk(), foliationRP2.foliationDisk(), 0.5)
{
}


std::ostream& balazs::operator<<(std::ostream& Out, const FoliationSphere& f){
    Out << "(Top Disk) " << f.topFoliation() << std::endl;
    Out << "(Bottom Disk) " << f.bottomFoliation() << std::endl;
    Out << "Twist: " << f.twist();

    return Out;
}


