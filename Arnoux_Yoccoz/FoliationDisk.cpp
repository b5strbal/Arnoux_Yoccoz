#include "FoliationDisk.h"

FoliationDisk::FoliationDisk(const WeighedTree& wt):
    m_intervalPairing(wt),
    m_weighedTree(wt)
{
}





std::ostream& operator<<(std::ostream& Out, const FoliationDisk& fd){
    using namespace std;

    Out << fd.m_intervalPairing.print() << endl;

    std::vector<int> singularityType = fd.m_weighedTree.getDegrees();

    Out << "Singularity type: " << singularityType;

    return Out;
}



