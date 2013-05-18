#ifndef FOLIATIONFROMSPHERE_H
#define FOLIATIONFROMSPHERE_H

#include "Foliation.h"

namespace balazs{

class FoliationSphere;

class FoliationFromSphereImpl{
    friend class FoliationFromSphere;

    FoliationFromSphereImpl(const FoliationSphere &foliationSphere);

    struct ConnectedPoints {
        Mod1Number topPoint;
        Mod1Number bottomPoint;
    };

    static void generateTopConnectingPairs(const FoliationSphere& foliationSphere,
                                           std::vector<ConnectedPoints>& allConnectedPoints);
    static void generateBottomConnectingPairs(const FoliationSphere& foliationSphere,
                                              std::vector<ConnectedPoints>& allConnectedPoints);

    static std::vector<long double> arg_lengths;
    static Permutation arg_permutation;
    static long double arg_twist;
};




class FoliationFromSphere : private FoliationFromSphereImpl, public Foliation
{
public:
    virtual ~FoliationFromSphere() {}
    FoliationFromSphere(const FoliationSphere &foliationSphere);
private:


};


}

#endif // FOLIATIONFROMSPHERE_H
