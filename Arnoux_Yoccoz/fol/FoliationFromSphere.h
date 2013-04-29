#ifndef FOLIATIONFROMSPHERE_H
#define FOLIATIONFROMSPHERE_H

#include "Foliation.h"
#include "FoliationSphere.h"

namespace balazs{

class FoliationFromSphere : public Foliation
{
public:
    static FoliationFromSphere fromFoliationSphere(const FoliationSphere &foliationSphere);
private:
    FoliationFromSphere(const std::vector<floating_point_type>& lengths,
                        const Permutation& permutation,
                        floating_point_type twist);

    struct ConnectedPoints {
        Mod1Number topPoint;
        Mod1Number bottomPoint;
    };


    static void generateTopConnectingPairs(const FoliationSphere& foliationSphere,
                                           std::vector<ConnectedPoints>& allConnectedPoints);
    static void generateBottomConnectingPairs(const FoliationSphere& foliationSphere,
                                              std::vector<ConnectedPoints>& allConnectedPoints);
};


}

#endif // FOLIATIONFROMSPHERE_H
