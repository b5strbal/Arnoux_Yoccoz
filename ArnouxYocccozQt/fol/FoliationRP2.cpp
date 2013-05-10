#include "FoliationRP2.h"
#include "../math/PerronFrobenius.h"

balazs::FoliationRP2::FoliationRP2(const FoliationDisk &fd)
    : m_foliationDisk(fd)
{
    if(foliationDisk().numSeparatrices() < 6){
        throw std::runtime_error("The foliation on the disk whose antipodal maps are identified to get a foliation "
                                 "on the projective plane has to correspond to a Weighed Tree with at least three "
                                 "edges, otherwise there is an immediate saddle connection.");
    }
}


balazs::FoliationRP2 balazs::arnouxYoccozRP2(){
    floating_point_type alpha = 1/arnouxYoccozStretchFactor(3);
    std::vector<floating_point_type> weighedTreeInput =
        { alpha + pow(alpha,2), pow(alpha,2) + pow(alpha, 3), pow(alpha, 3) + alpha };
    return FoliationRP2(WeighedTree(weighedTreeInput));
}


std::ostream& balazs::operator <<(std::ostream &out, const FoliationRP2 &f)
{
    out << f.foliationDisk();
    return out;
}


balazs::FoliationRP2 balazs::randomFoliationRP2(std::size_t numWeighedTreeEdges)
{
    return FoliationRP2(WeighedTree::randomWeighedTree(numWeighedTreeEdges));
}






