#include "FoliationDisk.h"

FoliationDisk::FoliationDisk(const WeighedTree& wt):
    m_intervalPairing(wt),
    m_weighedTree(wt)
{
}





std::ostream& operator<<(std::ostream& out, const FoliationDisk& d);
    out << d.m_intervalPairing << "\n";
    out << "Singularity type: " << d.m_weighedTree.getDegrees();
    return out;
}



