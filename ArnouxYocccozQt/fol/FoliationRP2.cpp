#include "FoliationRP2.h"
#include "../math/PerronFrobenius.h"

const long double balazs::FoliationRP2::alpha(1/balazs::arnouxYoccozStretchFactor(3));


balazs::FoliationRP2::FoliationRP2(const WeighedTree &wt)
    : m_foliationDisk(wt)
{
    if(foliationDisk().numSeparatrices() < 6){
        throw std::runtime_error("The foliation on the disk whose antipodal maps are identified to get a foliation "
                                 "on the projective plane has to correspond to a Weighed Tree with at least three "
                                 "edges, otherwise there is an immediate saddle connection.");
    }
}




balazs::FoliationRP2::FoliationRP2()
    : m_foliationDisk(WeighedTree({ alpha + pow(alpha,2), pow(alpha,2) + pow(alpha, 3), pow(alpha, 3) + alpha }))
{
}


balazs::FoliationRP2::FoliationRP2(std::size_t numWeighedTreeEdges)
    : FoliationRP2(WeighedTree::randomWeighedTree(numWeighedTreeEdges))
{
}






