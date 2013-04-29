#ifndef FOLIATIONFROMRP2_H
#define FOLIATIONFROMRP2_H

#include "FoliationFromSphere.h"
#include "FoliationRP2.h"

namespace balazs{


class FoliationFromRP2 : public FoliationFromSphere
{
public:
    FoliationFromRP2(const FoliationRP2& foliationRP2);
    static Foliation fromFoliationRP2(const FoliationRP2 &foliationRP2);
    const Permutation& intervalPermutationBeforeHalfTwist() const;
private:
    Permutation m_intervalPermutationBeforeHalfTwist;
};


}

#endif // FOLIATIONFROMRP2_H
