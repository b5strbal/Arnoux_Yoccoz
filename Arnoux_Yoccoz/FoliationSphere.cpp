#include "FoliationSphere.h"

FoliationSphere::FoliationSphere(const FoliationDisk& topFoliation, const FoliationDisk& bottomFoliation, floating_point_type twist) :
    m_topFoliation(topFoliation),
    m_bottomFoliation(bottomFoliation),
    m_twist(twist)
{
}


FoliationSphere::FoliationSphere(const FoliationRP2& foliationRP2) :
    FoliationSphere(foliationRP2.m_foliationDisk, foliationRP2.m_foliationDisk, 0.5)
{
}


std::ostream& operator<<(std::ostream& Out, const FoliationSphere& f){
    Out << "(Top Disk) " << f.topFoliation() << std::endl;
    Out << "(Bottom Disk) " << f.bottomFoliation() << std::endl;
    Out << "Twist: " << f.twist();

    return Out;
}

