#include "FoliationDisk.h"

FoliationDisk::FoliationDisk(const WeighedTree& wt):
    m_intervalPairing(wt),
    m_weighedTree(wt)
{
}





std::string print() const{
    std::ostringstream s;
    s << m_intervalPairing.print() << "\n";
    s << "Singularity type: " << m_weighedTree.getDegrees();
    return s.str();
}



