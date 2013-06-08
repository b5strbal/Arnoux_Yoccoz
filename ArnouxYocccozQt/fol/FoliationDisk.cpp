#include "FoliationDisk.h"



balazs::FoliationDisk::FoliationDisk(const WeighedTree& wt):
    m_intervalPairing(wt),
    m_weighedTree(wt)
{
    m_singularities = partition(singularityPermutation(m_intervalPairing));
}




