#include "FoliationRP2.h"
#include "PerronFrobenius.h"



FoliationRP2 FoliationRP2::arnouxYoccozRP2(){
    floating_point_type alpha = 1/arnouxYoccozStretchFactor(3);
    std::vector<floating_point_type> weighedTreeInput = { alpha + pow(alpha,2), pow(alpha,2) + pow(alpha, 3), pow(alpha, 3) + alpha };
    return FoliationRP2(WeighedTree(weighedTreeInput));
}







