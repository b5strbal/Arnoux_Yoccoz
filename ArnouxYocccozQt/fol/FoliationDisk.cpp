#include "FoliationDisk.h"



balazs::FoliationDisk::FoliationDisk(const WeighedTree& wt):
    m_intervalPairing(wt),
    m_weighedTree(wt)
{
}





std::ostream& balazs::operator<<(std::ostream& out, const FoliationDisk& d)
{
    out << d.m_intervalPairing << "\n";
    out << "Singularity type: " << d.m_weighedTree.degrees();
    return out;
}





