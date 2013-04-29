#include "FoliationRP2.h"
#include "../math/PerronFrobenius.h"

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







